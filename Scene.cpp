#include "stdafx.h"
#include "Scene.h"


Scene::Scene()
{
	rootNode = new GraphNode();

	GraphNode* nanosuitNode = new GraphNode();
	Model nanosuit("Models/Nanosuit/nanosuit.obj", nanosuitNode);
	rootNode->addChild(nanosuitNode);

	GraphNode* nanosuitNode2 = new GraphNode();
	Model nanosuit2("Models/Nanosuit/nanosuit.obj", nanosuitNode2);
	nanosuitNode->addChild(nanosuitNode2);

	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));
	Transform transformation(model);
	nanosuitNode->local = transformation;

	model = glm::mat4(1.0f);
	model = glm::translate(glm::vec3(10.0f, 0.0f, 0.0f));
	transformation.transformation = model;
	nanosuitNode2->local = transformation;
}


Scene::~Scene()
{
}

void Scene::Render(Shader* shader)
{
	rootNode->render(shader->programHandle, Transform::origin(), shader);
}