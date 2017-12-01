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
	rootNode->addChild(nanosuitNode2);

	/*glm::mat4 model = glm::mat4(1.0f);*/
	//model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(glm::vec3(-10.0f, 0.0f, -50.0f));
	transformation.transformation = model;
	nanosuitNode->local = transformation;

	model = glm::mat4(1.0f);
	model = glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));
	model = glm::translate(glm::vec3(translateX, translateY, translateZ));
	transformation.transformation = model;
	nanosuitNode2->local = transformation;

	/*GUI*/
	TwBar* bar = TwNewBar("Model administration");

	TwAddVarRW(bar, "Translate_X", TW_TYPE_FLOAT, &translateX, "");
	TwAddVarRW(bar, "Translate_Y", TW_TYPE_FLOAT, &translateY, "");
	TwAddVarRW(bar, "Translate_Z", TW_TYPE_FLOAT, &translateZ, "");
}


Scene::~Scene()
{
}

void Scene::Render(Shader* shader)
{
	model = glm::translate(glm::vec3(translateX, translateY, translateZ));
	for each(GraphNode* child in rootNode->children[0]->children)
	{
		child->local = model;
	}
	rootNode->render(shader->programHandle, Transform::origin(), shader);
}