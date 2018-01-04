#include "stdafx.h"
#include "Scene.h"


Scene::Scene(Shader* shader)
{
	this->shader = shader;

	rootNode = new GraphNode();

	//GraphNode* nanosuitNode = new GraphNode();
	//Model nanosuit("Models/Nanosuit/nanosuit.obj", nanosuitNode);
	//rootNode->addChild(nanosuitNode);

	//GraphNode* nanosuitNode2 = new GraphNode();
	//Model nanosuit2("Models/Nanosuit/nanosuit.obj", nanosuitNode2);
	//rootNode->addChild(nanosuitNode2);

	///*glm::mat4 model = glm::mat4(1.0f);*/
	////model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::translate(glm::vec3(-10.0f, 0.0f, -50.0f));
	//transformation.transformation = model;
	//nanosuitNode->local = transformation;

	//model = glm::mat4(1.0f);
	//model = glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));
	//model = glm::translate(glm::vec3(translateX, translateY, translateZ));
	//transformation.transformation = model;
	//nanosuitNode2->local = transformation;

	GraphNode* nanosuitNode = new GraphNode();
	Model nanosuit("Models/Nanosuit/nanosuit.obj", nanosuitNode);
	rootNode->addChild(nanosuitNode);
	nanosuitNode->local.CalculateWorldMatrix();

	GraphNode* nanosuitNode2 = new GraphNode();
	Model nanosuit2("Models/Nanosuit/nanosuit.obj", nanosuitNode2);
	rootNode->addChild(nanosuitNode2);
	nanosuitNode2->local.translation = glm::vec3(10.0f, 5.0f, 10.0f);
	nanosuitNode2->local.CalculateWorldMatrix();

	GraphNode* nanosuitNode3 = new GraphNode();
	Model nanosuit3("Models/Nanosuit/nanosuit.obj", nanosuitNode3);
	rootNode->addChild(nanosuitNode3);
	nanosuitNode3->local.translation = glm::vec3(10.0f, 5.0f, 10.0f);
	nanosuitNode3->local.CalculateWorldMatrix();

	rootNode->local.translation = glm::vec3(0.0f, 0.0f, -20.0f);
}

Scene::~Scene()
{
}

void Scene::Render()
{
	rootNode->render(shader->programHandle, Transform::origin(), shader);
}