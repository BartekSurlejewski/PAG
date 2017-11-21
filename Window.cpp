#include "stdafx.h"
#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm\gtx\transform.hpp>
#include <iostream>

Window::Window()
{
}


Window::~Window()
{
}

GLFWwindow* Window::getWindow()
{
	return window;
}

int Window::init(int width, int height)
{
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(width, height, "Hello 3D!", nullptr, nullptr);

	if(window == nullptr)
	{
		std::cout << "Failed to create a window." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, width, height);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);

	return true;
}
