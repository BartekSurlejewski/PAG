#include "stdafx.h"
#include "Scene.h"


Scene::Scene()
{
	rootNode = new GraphNode();

	Model nanosuit("Models/Nanosuit/nanosuit.obj", rootNode);
	//Model nanosuit2("Models/Nanosuit/nanosuit.obj", rootNode);

	/*glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	Transform transformation(model);
	nanosuit.SetTransform(transformation);

    model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	transformation.transformation = model;
	nanosuit2.SetTransform(transformation);*/
}


Scene::~Scene()
{
}

void Scene::Render(Shader* shader)
{
	rootNode->render(shader->programHandle, Transform::origin(), shader);
}