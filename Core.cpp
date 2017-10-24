#include "stdafx.h"
#include "Core.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

Core::Core(Window* window)
{
	screen = window;
}


Core::~Core()
{
}

void Core::processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Core::render(float tpf)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Core::update()
{
	float oldTime = 0.0f;
	float newTime = 0.0f;
	float gameTime = 0.0f;

	GLFWwindow* window = screen->getWindow();

	while (!glfwWindowShouldClose(window))
	{
		oldTime = newTime;
		newTime = (float)glfwGetTime();
		gameTime = newTime - oldTime;

		processInput(window);

		render(gameTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}