#include "stdafx.h"
#include "Transform.h"

glm::vec3 scales[] = {
						glm::vec3(1.5, 1.5, 1.5),
						glm::vec3(1, 1, 1),
						glm::vec3(1, 1, 1),
						glm::vec3(0.5, 0.5, 0.5),
						glm::vec3(0.6, 0.6, 0.6),
						glm::vec3(1, 1, 1),
						glm::vec3(0.5, 0.5, 0.5),
						glm::vec3(1, 1, 1),
						glm::vec3(0.6, 0.6, 0.6),
						glm::vec3(0.3, 0.3, 0.3)
};

//void Transform::move(GLuint programHandle, int objectNumber)
//{
//	model = glm::mat4(1.0f);
//	model = glm::scale(model, scales[objectNumber]);
//	model = glm::rotate(model, ((float)objectNumber + (float)0.0001) * (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
//	model = glm::translate(model, Mesh::cubePositions[objectNumber]);
//	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
//	GLuint modelLoc = glGetUniformLocation(programHandle, "model");
//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//}

Transform Transform::origin()
{
	Transform transform;
	transform.transformation = glm::mat4(1.0f);
	return transform;
}

Transform Transform::combine(Transform& other)
{
	Transform transform;
	transform.transformation = other.transformation * this->transformation;

	return transform;
}