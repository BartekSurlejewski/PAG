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

const int SCR_WIDTH = 1000;
const int SCR_HEIGHT = 640;
const std::string texturePath = "Brickwall_texture.jpg";

int main()
{
	Window* window = new Window();
	Mesh mesh;
	Texture texture;
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
	Camera* camera= new Camera(programHandle, window);
	Core core(window, camera);

	texture.loadTexture(texturePath);

	Shader shader(programHandle, "Shaders/basic.vert", "Shaders/basic.frag");

	core.update(programHandle, shader, texture, transform);

	glfwTerminate();
	return 0;
}