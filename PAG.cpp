#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <fstream>
#include <string>

#include "Window.h"
#include "Mesh.h"
#include "Core.h"
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION

const int SCR_WIDTH = 640;
const int SCR_HEIGHT = 480;

GLuint VBO = NULL;

void initShader()
{
	Shader shader;

	/*Shader init*/
	GLuint programHandle = glCreateProgram();

	if (programHandle == 0)
	{
		std::cout << "Error creating program object" << std::endl;
	}

	shader.loadAndCompileShaderFromFile(GL_VERTEX_SHADER, "Shaders/basic.vert", programHandle);
	shader.loadAndCompileShaderFromFile(GL_FRAGMENT_SHADER, "Shaders/basic.frag", programHandle);

	glLinkProgram(programHandle);

	glUseProgram(programHandle);
}

int main()
{
	Window* screen = new Window();
	Mesh mesh;
	Core core(screen);

	if (!screen->init(SCR_WIDTH, SCR_HEIGHT)) 
	{
		return -1;
	}

	if (!mesh.loadContent(1, VBO))
	{
		return -1;
	}
	
	initShader();

	core.update();

	glfwTerminate();
	return 0;
}