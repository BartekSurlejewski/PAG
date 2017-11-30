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

Core::Core(Window* window, Camera* camera, Shader shader)
{
	screen = window;
	this->camera = camera;

	GLuint transformLoc;
	glm::mat4 trans = glm::mat4(1.0f);
	transformLoc = glGetUniformLocation(shader.programHandle, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
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

void Core::update(GLuint programHandle, Shader shader, GraphNode* rootNode, Model nanosuit)
{
	GLFWwindow* window = screen->getWindow();
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		camera->update(programHandle, screen, deltaTime);
		shader.use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		Transform transformation(model);
		nanosuit.SetTransform(transformation);
		nanosuit.Render(&shader);

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