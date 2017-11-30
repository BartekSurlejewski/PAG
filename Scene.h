#include "Model.h"
#include "Shader.h"
#include "GraphNode.h"

#pragma once
class Scene
{
public:
	Scene();
	~Scene();
	void Render(Shader* shader);

private:
	vector<Model> models;
	Shader* shader;
	GraphNode* rootNode;
};

