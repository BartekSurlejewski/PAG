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
	void updateVectors();
	void processKeyboard(int);
	void processMouseMovement(float, float, GLboolean);

	GLfloat yaw = -90.0f;
	GLfloat pitch = 0.0f;

private:
	int height, width;
	glm::mat4 world, view, projection, WVP;
	glm::vec3 cameraPos, cameraFront, cameraUp;

	GLfloat deltaTime;
	GLfloat movementSpeed = 4.0f;
	GLfloat mouseSensitivity = 0.1f;
};

