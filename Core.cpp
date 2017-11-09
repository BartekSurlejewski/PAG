#include "stdafx.h"
#include "Core.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

std::string textures[] = {
						"Brickwall_texture.jpg",
						"someTexture.jpg",
						"images.jpg"
};

Core::Core(Window* window, Camera* camera)
{
	screen = window;
	this->camera = camera;
}

Core::~Core()
{
}

void Core::processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->processKeyboard(GLFW_KEY_W);
	else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->processKeyboard(GLFW_KEY_S);
	else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->processKeyboard(GLFW_KEY_A);
	else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->processKeyboard(GLFW_KEY_D);

	glfwGetCursorPos(window, &xpos, &ypos);
	processMouseMovement();
}

void Core::update(GLuint programHandle, Texture texture, Transform transform)
{
	GLFWwindow* window = screen->getWindow();
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		camera->update(programHandle, screen, deltaTime);
		for (int i = 0; i < 10; i++)
		{
			texture.loadTexture(textures[i % 3]);
			transform.move(programHandle, i);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Core::processMouseMovement()
{
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
	camera->processMouseMovement(xoffset, yoffset, true);
}