#include "Model.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <float.h>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int TextureFromFile(const char *path, const std::string &directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		//std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

Model::Model()
{

}

void Model::LoadModel(const std::string & path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs);

	if (!scene || !scene->mRootNode)
	{
		__debugbreak();
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	ModelNode* rootNode = new ModelNode();
	graph.rootNode = rootNode;

	processNode(scene->mRootNode, scene, rootNode);
}
void Model::Draw(Shader & shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::DrawAsGraph(Shader & shader)
{
	Transform xForm;
	xForm.CalculateWorldMatrix();

	graph.rootNode->Draw(xForm, &shader);
}

void Model::processNode(aiNode * node, const aiScene * scene, ModelNode * sceneNode)
{
	const unsigned int numberMeshes = node->mNumMeshes;

	for (unsigned int i = 0; i < numberMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	glm::mat4 matXFormNode;
	memcpy(&matXFormNode, &node->mTransformation, sizeof(aiMatrix4x4));
	matXFormNode = glm::transpose(matXFormNode);
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(matXFormNode, scale, rotation, translation, skew, perspective);

	sceneNode->xForm.scale = scale;
	sceneNode->xForm.translate = translation;
	sceneNode->xForm.CalculateWorldMatrix();
	sceneNode->name = std::string(node->mName.C_Str());

	for (int i = numberMeshes; i > 0; i--)
	{
		Mesh* mesh = &meshes[meshes.size() - i];
		sceneNode->meshes = &meshes;
		sceneNode->meshesID.push_back(meshes.size() - 1);
	}

	for (unsigned int i = 0; i < sceneNode->meshesID.size(); i++)
	{
		const unsigned int meshID = sceneNode->meshesID[i];
		Mesh* mesh = &meshes[meshID];

		glm::vec3 meshMin, meshMax;
		mesh->GetBoundingBox(meshMin, meshMax);
		glm::vec3& min = sceneNode->boundingBoxMin;
		glm::vec3& max = sceneNode->boundingBoxMax;

		if (meshMin.x < min.x) min.x = meshMin.x;
		if (meshMin.y < min.y) min.y = meshMin.y;
		if (meshMin.z < min.z) min.z = meshMin.z;

		if (meshMax.x > max.x) max.x = meshMax.x;
		if (meshMax.y > max.y) max.y = meshMax.y;
		if (meshMax.z > max.z) max.z = meshMax.z;
	}

	sceneNode->boundingSphereCenter = (sceneNode->boundingBoxMin + sceneNode->boundingBoxMax) * 0.5f;
	sceneNode->boundingSphereRadius = glm::distance(sceneNode->boundingBoxMax, sceneNode->boundingSphereCenter);

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ModelNode* childSceneNode = new ModelNode();
		sceneNode->children.push_back(childSceneNode);

		processNode(node->mChildren[i], scene, childSceneNode);
	}
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<MeshTexture> textures;

	glm::vec3 min(FLT_MAX);
	glm::vec3 max(FLT_MIN);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		if (vertex.Position.x < min.x) min.x = vertex.Position.x;
		if (vertex.Position.y < min.y) min.y = vertex.Position.y;
		if (vertex.Position.z < min.z) min.z = vertex.Position.z;

		if (vertex.Position.x > max.x) max.x = vertex.Position.x;
		if (vertex.Position.y > max.y) max.y = vertex.Position.y;
		if (vertex.Position.z > max.z) max.z = vertex.Position.z;

		if (mesh->HasNormals())
		{
			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace& face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	Mesh finalMesh = Mesh(vertices, indices, textures);
	finalMesh.SetBoundingBox(min, max);

	return finalMesh;
}

std::vector<MeshTexture> Model::loadMaterialTextures(aiMaterial * material, aiTextureType type, const std::string & typeName)
{
	std::vector<MeshTexture> textures;

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);

		MeshTexture tex;
		tex.id = TextureFromFile(str.C_Str(), directory);
		tex.type = typeName;

		textures.push_back(tex);
	}

	return textures;
}
