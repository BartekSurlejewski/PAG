#include "stdafx.h"
#include "Transform.h"


Transform::Transform()
{
}


Transform::~Transform()
{
}

void Transform::move(GLuint programHandle)
{
	model = glm::mat4(1.0f);
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	GLuint modelLoc = glGetUniformLocation(programHandle, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}
