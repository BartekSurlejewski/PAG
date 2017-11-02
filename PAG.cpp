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
#include "Camera.h"
#include "Transform.h"
#include "Scene.h"
#include "stb_image.h"

const int SCR_WIDTH = 640;
const int SCR_HEIGHT = 480;
const std::string texturePath = "Brickwall_texture.jpg";

int main()
{
	Window* window = new Window();
	Mesh mesh;
	Texture texture;
	Core core(window);
	Camera camera;
	Transform transform;

	if (!window->init(SCR_WIDTH, SCR_HEIGHT)) 
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

	glEnable(GL_DEPTH_TEST);

	core.update(programHandle, shader, texture, transform, camera);

	glfwTerminate();
	return 0;
}