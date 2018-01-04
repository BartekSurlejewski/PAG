#include "stdafx.h"
#include "Model.h"
#include <glm/gtx/matrix_decompose.hpp>

void Model::Render(Shader* shader)
{
	GLuint transformLoc = glGetUniformLocation(shader->programHandle, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform.transformation));

	for (GLuint i = 0; i < meshes.size(); i++)
	{
			meshes[i].Draw(shader);
	}

	if (nodes.size() > 0)
	{
		for each(Model* node in nodes)
			node->Draw(shader);
	}
}

void Model::SetNode(Model* node)
{
	nodes.push_back(node);
}

void Model::SetTransform(Transform newTransform)
{
	transform = newTransform;
}

void Model::Draw(Shader* shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::loadModel(string path, GraphNode* node)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene, node);
}

void Model::processNode(aiNode *node, const aiScene *scene, GraphNode* graphNode)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for ( unsigned int i = node->mNumMeshes; i > 0; i--)
	{
		Mesh* pMesh = &meshes[meshes.size() - i];

		graphNode->meshes = meshes;
		graphNode->meshesIds.push_back(meshes.size() - 1);
	}

	glm::mat4 matXformNode;
	memcpy(&matXformNode, &node->mTransformation, sizeof(aiMatrix4x4));
	matXformNode = glm::transpose(matXformNode);
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translate;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(matXformNode, scale, rotation, translate, skew, perspective);

	graphNode->local.scale = scale;
	graphNode->local.translation = translate;
	graphNode->local.CalculateWorldMatrix();

		// Calculate AABB for this node from its meshes' AABBs.
		for (unsigned int i = 0; i < graphNode->meshesIds.size(); i++)
		{
			const unsigned int meshID = graphNode->meshesIds[i];
			Mesh* pMesh = &meshes[meshID];

			// mesh min/max
			glm::vec3 vMeshMin, vMeshMax;
			pMesh->GetAABB(vMeshMin, vMeshMax);

			// node min/max
			glm::vec3& vMin = graphNode->AABBMin;
			glm::vec3& vMax = graphNode->AABBMax;

			// calculate aabb
			if (vMeshMin.x < vMin.x)
				vMin.x = vMeshMin.x;
			if (vMeshMin.y < vMin.y)
				vMin.y = vMeshMin.y;
			if (vMeshMin.z < vMin.z)
				vMin.z = vMeshMin.z;

			if (vMeshMax.x > vMax.x)
				vMax.x = vMeshMax.x;
			if (vMeshMax.y > vMax.y)
				vMax.y = vMeshMax.y;
			if (vMeshMax.z > vMax.z)
				vMax.z = vMeshMax.z;
		}

	graphNode->boundingSphereCenter = (graphNode->AABBMin + graphNode->AABBMax) * 0.5f;
	graphNode->boundingSphereRadius = glm::distance(graphNode->AABBMax, graphNode->boundingSphereCenter);

	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		GraphNode* child = new GraphNode();
		graphNode->addChild(child);
		processNode(node->mChildren[i], scene, child);
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	glm::vec3 min(FLT_MIN);
	glm::vec3 max(FLT_MAX);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex positions, normals and texture coordinates
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		if (vertex.Position.x < min.x) min.x = vertex.Position.x;
		if (vertex.Position.y < min.y) min.y = vertex.Position.y;
		if (vertex.Position.z < min.z) min.z = vertex.Position.z;

		if (vertex.Position.x > max.x) max.x = vertex.Position.x;
		if (vertex.Position.y > max.y) max.y = vertex.Position.y;
		if (vertex.Position.z > max.z) max.z = vertex.Position.z;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process material
		if (mesh->mMaterialIndex >= 0)
		{
			if (mesh->mMaterialIndex >= 0)
			{
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
			}
		}

	Mesh resultMesh(vertices, indices, textures);
	resultMesh.SetAABB(min, max);

	return resultMesh;
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = this->TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}

unsigned int Model::TextureFromFile(const char *path, const string &directory)
{
	string filename = string(path);
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
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}