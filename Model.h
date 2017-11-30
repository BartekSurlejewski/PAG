#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include <vector>

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GraphNode.h"
#include "Texture.h"
#include "Transform.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#pragma once

class Model
{
public:
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	vector<Model*> nodes;
	string directory;
	

	Model::Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
	{
		loadModel(path);
	}

	Model::~Model()
	{
	}

	void Render(Shader* shader);
	void SetNode(Model* node);
	void SetTransform(Transform);

	void Draw(Shader* shader);
private:
	Texture* texture;
	Transform transform;
	bool gammaCorrection;

	void loadModel(string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const char *path, const string &directory);
};

