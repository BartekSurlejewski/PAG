#define STB_IMAGE_IMPLEMENTATION

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
#include "Texture.h"
#include "stb_image.h"

const int SCR_WIDTH = 640;
const int SCR_HEIGHT = 480;
const std::string texturePath = "texture.jpg";

int main()
{
	Window* screen = new Window();
	Mesh mesh;
	Core core(screen);
	Texture texture;
	

	if (!screen->init(SCR_WIDTH, SCR_HEIGHT)) 
	{
		return -1;
	}

	if (!mesh.loadContent(1))
	{
		return -1;
	}

	GLuint programHandle = glCreateProgram();

	texture.loadTexture(texturePath);

	Shader shader(programHandle, "Shaders/basic.vert", "Shaders/basic.frag");

	core.update(programHandle);

	glfwTerminate();
	return 0;
}