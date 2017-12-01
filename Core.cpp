#include "stdafx.h"
#include "Core.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <AntTweakBar/AntTweakBar.h>
#include <string>
#include <iostream>

std::string textures[] = {
						"Brickwall_texture.jpg",
						"someTexture.jpg",
						"images.jpg"
};

Camera* globalCamera = nullptr;
float xOffset = 0.0f;
float yOffset = 0.0f;
bool isCursorEnabled;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(!TwEventMousePosGLFW((int)xpos, (int)ypos))
	{
		static float lastX = 1000 / 2;
		static float lastY = 640 / 2;

		xOffset = xpos - lastX;
		yOffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;
		float sensitivity = 0.05;
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		globalCamera->yaw += xOffset;
		globalCamera->pitch += yOffset;
		globalCamera->updateVectors();
	}
}

void mouse_button(GLFWwindow* window, int button, int x, int y)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && x==0)
	{
		if(isCursorEnabled)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		isCursorEnabled = !isCursorEnabled;
	}
	else if (!TwEventMouseButtonGLFW(button, x))
	{
		std::cout << "I'm terribly sorry, I couldn't handle this event. :(" << std::endl;
	}
}

Core::Core(Window* window, Camera* camera, Shader shader)
{
	globalCamera = camera;
	this->screen = window;
	this->camera = camera;	
	scene = new Scene();
	isCursorEnabled = window->isCursorEnabled;

	glfwSetCursorPosCallback(window->getWindow(), mouse_callback);
	glfwSetMouseButtonCallback(window->getWindow(), mouse_button);

	glm::mat4 transform = glm::mat4(1.0f);
	GLuint transformLoc = glGetUniformLocation(shader.programHandle, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
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
	//glfwGetCursorPos(window, &xpos, &ypos);
}

void Core::update(GLuint programHandle, Shader shader)
{
	GLFWwindow* window = screen->getWindow();
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		camera->update(programHandle, screen, deltaTime);
		shader.use();

		scene->Render(&shader);

		TwDraw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

//void Core::processMouseMovement()
//{
//	GLfloat xoffset = xpos - lastX;
//	GLfloat yoffset = lastY - ypos;
//
//	lastX = xpos;
//	lastY = ypos;
//	camera->processMouseMovement(xoffset, yoffset, true);
//}