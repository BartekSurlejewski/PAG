#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"
#include "Shader.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Model.h"

class Bloom
{
public:
	Bloom();
	~Bloom();
	void Initialize(Shader, Shader, Shader, Shader);
	void Render();

	unsigned int hdrFBO;
	unsigned int colorBuffers[2];
	unsigned int pingpongFBO[2];
	unsigned int pingpongBuffer[2];
	std::vector<glm::vec3> lightPositions = { glm::vec3(5.0f, 9.5f, -62.0f),
											glm::vec3(15.0f, 10.0f, -62.0f),
											glm::vec3(25.5f, 10.0, -62.0f)
											};

	std::vector<glm::vec3> lightColors = { glm::vec3(2.0f, 2.0f, 2.0f),
										glm::vec3(50.0f, 0.0f, 0.0f),
										glm::vec3(1.0f, 0.0f, 1.5f)
										};

private:
	Shader bloomShader;
	Shader blurShader;
	Shader lightShader;
	Shader finalBloomShader;

	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	glm::mat4 model;

	Model cubeRed, cubeGreen, cubeBlue;

	void renderQuad();
	void renderCube();
};

