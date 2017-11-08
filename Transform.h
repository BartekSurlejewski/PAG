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
	Transform();
	~Transform();
	void move(GLuint, int);

private:
	glm::mat4 model;
};

