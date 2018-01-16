#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform
{
	Transform() : translate(0.0f, 0.0f, 0.0f), scale(1.0f), rotate(0.0f, 0.0f, 0.0f)
	{
		CalculateWorldMatrix();
	}

	glm::vec3 translate;
	glm::vec3 scale;
	glm::vec3 rotate;

	glm::mat4 worldMatrix;

	void CalculateWorldMatrix()
	{
		worldMatrix = glm::mat4(1);
		worldMatrix = glm::translate(worldMatrix, translate);
		worldMatrix = glm::rotate(worldMatrix, glm::radians(rotate.x), glm::vec3(1.0, 0.0, 0.0));
		worldMatrix = glm::rotate(worldMatrix, glm::radians(rotate.y), glm::vec3(0.0, 1.0, 0.0));
		worldMatrix = glm::rotate(worldMatrix, glm::radians(rotate.z), glm::vec3(0.0, 0.0, 1.0));
		worldMatrix = glm::scale(worldMatrix, scale);
	}
};