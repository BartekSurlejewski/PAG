#define GLFW_INCLUDE_NONE

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glad\glad.h>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Mesh.h"

#pragma once
class Transform
{
public:
	Transform(glm::mat4 transformation): translation(0.0f, 0.0f, 0.0f), scale(1.0f), rotation(0.0f, 0.0f, 0.0f)
	{
		this->transformation = transformation;
	}
	Transform(): translation(0.0f, 0.0f, 0.0f), scale(1.0f), rotation(0.0f, 0.0f, 0.0f)
	{
		transformation = glm::mat4(1.0f);
	}
	~Transform(){}
	Transform combine(Transform& other);
	void CalculateWorldMatrix();

	static Transform origin();

	glm::mat4 transformation;

	glm::vec3 translation;
	glm::vec3 scale;
	glm::vec3 rotation;
};

