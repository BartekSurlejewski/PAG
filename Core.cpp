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

void Core::render(Shader shader, Texture texture)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Core::update(GLuint programHandle, Shader shader, Texture texture, Transform transform, Camera camera)
{
	GLFWwindow* window = screen->getWindow();

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		int vertexColorLocation = glGetUniformLocation(programHandle, "ourColor");

		camera.init(programHandle, screen);
		transform.move(programHandle);

		render(shader, texture);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}