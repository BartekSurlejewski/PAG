#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Window.h"

#pragma once
class Camera
{
public:
	Camera();
	~Camera();
	void init(GLuint, Window*);

private:
	int height, width;
	glm::mat4 view, projection;
};

