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
#include "GraphNode.h"
#include "Model.h"
#include "UI.h"

const int SCR_WIDTH = 1000;
const int SCR_HEIGHT = 640;
const std::string texturePath_Brick = "Brickwall_texture.jpg";
const std::string texturePath_Earth = "earth.jpg";
const std::string texturePath_Moon = "moon.jpg";

int main()
{
	Window* window = new Window();
	if (!window->init(SCR_WIDTH, SCR_HEIGHT)) 
	{
		return -1;
	}

	UI ui(SCR_WIDTH, SCR_HEIGHT, window->getWindow());

	GLuint programHandle = glCreateProgram();

	Shader shader(programHandle, "Shaders/basic.vert", "Shaders/basic.frag");

	Camera* camera= new Camera(programHandle, window);
	Core core(window, camera, shader);

	core.update(programHandle, shader);

	TwTerminate();
	glfwTerminate();
	return 0;
}