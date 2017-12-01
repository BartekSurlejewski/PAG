#include "Model.h"
#include "Shader.h"
#include "GraphNode.h"
#include <AntTweakBar/AntTweakBar.h>

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
	Transform transformation;
	glm::mat4 model = glm::mat4(1.0f);
	float translateX = 10, translateY = -10, translateZ = -50;
};

