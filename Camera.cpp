#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::init(GLuint programHandle, Window* window)
{
	/*View matrix*/
	view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(1.5f, 0.0f, 1.5f),  // camera position in world space
								glm::vec3(0.0f, 0.0f, 0.0f),  // at this point camera is looking
								glm::vec3(0.0f, 1.0f, 0.0f)); // head is up
	
    /*Set projection matrix*/
	glfwGetWindowSize(window->getWindow(), &width, &height);
	projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f);

	GLuint viewLoc = glGetUniformLocation(programHandle, "view");
	GLuint projectionLoc = glGetUniformLocation(programHandle, "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
