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
	Model model, model2, plane, cube;

	Window window;
	Shader shader;
	Shader depthShader;
	Shader debugDepthShader;
	Shader shaderSphere;
	Shader shadowShader;
	Model modelSphere;
	bool canMoveCamera;

	DirectionalLight* directionalLight = nullptr;
	PointLight* pointLight = nullptr;
	SpotLight* spotLight = nullptr;


private:
	void processInputCore(GLFWwindow* window, float dt);
	void Render();
	void DrawScene(Shader);
	void SetLights();
	void renderQuad();
	void renderCube();

	Camera camera;

	glm::vec3 directionalLightPos;

	float lastTime;
	unsigned int depthMapFBO;
	unsigned int depthMap;
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
};