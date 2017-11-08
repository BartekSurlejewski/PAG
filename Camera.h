#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Window.h"

#pragma once
class Camera
{
public:
	Camera(GLuint, Window*);
	~Camera();
	void update(GLuint, Window*, GLfloat);
	void processKeyboard(int);

private:
	int height, width;
	GLfloat deltaTime;
	glm::mat4 view, projection;
	glm::vec3 cameraPos, cameraFront, cameraUp;
};

