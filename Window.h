#pragma once


#include <iostream>

class GLFWwindow;

class Window
{
public:
	Window();
	GLFWwindow * GetWindow();
	void Clear();
	bool WindowActive();
	void Swap();
	void Initialization(int width, int height);

private:
	GLFWwindow * window;
};
