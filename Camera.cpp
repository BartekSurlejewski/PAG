#include "stdafx.h"
#include "Camera.h"


Camera::Camera(GLuint programHandle, Window* window)
{
	cameraPos = glm::vec3(1.5f, 0.0f, 3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::mat4(1.0f);

	/*Set projection matrix*/
	glfwGetWindowSize(window->getWindow(), &width, &height);
	projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f);
}


Camera::~Camera()
{
}

void Camera::update(GLuint programHandle, Window* window, GLfloat deltaTime)
{
	/*View matrix*/
	this->deltaTime = deltaTime;
	view = glm::mat4(1.0f);
	view = glm::lookAt(	cameraPos,					// camera position in world space
						cameraPos + cameraFront,	// at this point camera is looking
						cameraUp);					// head is up

	GLuint viewLoc = glGetUniformLocation(programHandle, "view");
	GLuint projectionLoc = glGetUniformLocation(programHandle, "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::moveCamera(int key)
{
	float cameraSpeed = 4.0f * deltaTime;

	if (key == GLFW_KEY_W)
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	else if (key == GLFW_KEY_S)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	else if (key == GLFW_KEY_A)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *  cameraSpeed;
	}
	else if (key == GLFW_KEY_D)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *  cameraSpeed;
	}
}