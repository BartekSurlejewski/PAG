#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
public:
	Window();
	~Window();
	int init(int width, int height);

	bool isCursorEnabled;
	GLFWwindow* getWindow();

private:
	GLFWwindow* window;
};

