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
//#include "stb_image.h"

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

	GLuint programHandle = glCreateProgram();

	Shader shader(programHandle, "Shaders/basic.vert", "Shaders/basic.frag");

	GraphNode* rootNode = new GraphNode(NULL);
	rootNode->render(programHandle, Transform::origin(), true, shader);

	Camera* camera= new Camera(programHandle, window);
	Core core(window, camera, shader);

	Model nanosuit("Models/Nanosuit/nanosuit.obj");
	//Model boxGreen("Models/CubeGreen/CubeGreen.obj");

	core.update(programHandle, shader, rootNode, nanosuit);

	glfwTerminate();
	return 0;
}