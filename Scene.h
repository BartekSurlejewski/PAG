#include "Model.h"
#include "Shader.h"
#include "GraphNode.h"
#include <glm\gtc\type_ptr.hpp>
#include <AntTweakBar/AntTweakBar.h>

#pragma once
class Scene
{
public:
	Scene(Shader* shader);
	~Scene();
	void Render();
	GraphNode* rootNode;

private:
	vector<Model> models;
	Shader* shader;
	Transform transformation;
	glm::mat4 model = glm::mat4(1.0f);
	float translateX = 10, translateY = -10, translateZ = -50;
	float rotateX = 0, rotateY = 0, rotateZ = 0;
};

