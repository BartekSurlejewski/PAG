#pragma once

#include "Mesh.h"
#include "ModelNode.h"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

class Model
{
public:
	Model();
	void LoadModel(const std::string& path);
	void Draw(Shader& shader);
	void DrawAsGraph(Shader& shader);

	SceneGraph graph;

private:
	void processNode(aiNode* node, const aiScene* scene, ModelNode* sceneNode);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<MeshTexture> loadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName);

	std::vector<Mesh> meshes;
	std::string directory;
};