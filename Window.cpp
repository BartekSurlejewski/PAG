#include "stdafx.h"
#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

	window = glfwCreateWindow(width, height, "Hello triangle!", nullptr, nullptr);

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
	//glViewport(width/4, height/4, width/2, height/2);

	return true;
}
