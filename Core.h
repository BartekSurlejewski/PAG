#pragma once
#define GLFW_INCLUDE_NONE

#include "Window.h"
#include "Camera.h"
#include "Transform.h"
#include "Model.h"
#include <AntTweakBar/AntTweakBar.h>

class DirectionalLight;
class PointLight;
class SpotLight;

class Core
{
public:
	Core();
	bool Initialize();
	void Update();

	Camera* GetCamera() { return &camera; }
	TwBar* bar;
	TwBar* barLighting;
	Model model, model2, plane;

	Window window;
	Shader shader;
	Shader shaderSphere;
	Model modelSphere;
	bool canMoveCamera;

	DirectionalLight* directionalLight = nullptr;
	PointLight* pointLight = nullptr;
	SpotLight* spotLight = nullptr;


private:
	void processInputCore(GLFWwindow* window, float dt);
	void Render();
	void DrawScene();
	void SetLights();

	Camera camera;

	float lastTime;
	unsigned int depthMapFBO;
};