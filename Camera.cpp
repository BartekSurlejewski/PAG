#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
	// set default values
	position = glm::vec3(0.f, 0.f, 3.f);
	upVector = glm::vec3(0.f, 1.f, 0.f);
	frontVector = glm::vec3(0.f, 0.f, -1.f);

	yaw = -90.f;
	pitch = 0.0f;

	aspectRatio = 16.f / 9.f; // ~1.777778
	UpdatePerspectiveMatrix();
}

void Camera::SetAspectRatio(float newAspectRatio)
{
	aspectRatio = newAspectRatio;
	UpdatePerspectiveMatrix();
}

void Camera::UpdateOrientation()
{
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		frontVector = glm::normalize(front);

		UpdatePerspectiveMatrix();
	}
}

void Camera::processInput(GLFWwindow* window, float dt)
{
	float cameraSpeed = 20.0f * dt;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += cameraSpeed * frontVector;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= cameraSpeed * frontVector;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position -= glm::normalize(glm::cross(frontVector, upVector)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += glm::normalize(glm::cross(frontVector, upVector)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		position.y += cameraSpeed * 2;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		position.y -= cameraSpeed * 2;
	}

	//glm::mat4
	view = glm::lookAt(position, position + frontVector, upVector);

}



void Camera::UpdatePerspectiveMatrix()
{
	projection = glm::perspective(glm::radians(75.f), aspectRatio, 0.1f, 500.f);
}
