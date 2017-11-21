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

void Camera::processKeyboard(int key)
{
	GLfloat velocity = movementSpeed * deltaTime;

	if (key == GLFW_KEY_W)
	{
		cameraPos += velocity * cameraFront;
	}
	else if (key == GLFW_KEY_S)
	{
		cameraPos -= velocity * cameraFront;
	}
	else if (key == GLFW_KEY_A)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *  velocity;
	}
	else if (key == GLFW_KEY_D)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *  velocity;
	}

	cameraPos.y = 0;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateVectors();
}

void Camera::updateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}