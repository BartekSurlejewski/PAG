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
	Transform(glm::mat4 transformation)
	{
		this->transformation = transformation;
	}
	Transform()
	{
		transformation = glm::mat4(1.0f);
	}
	~Transform(){}
	Transform combine(Transform& other);

	static Transform origin();

	glm::mat4 transformation;
};

