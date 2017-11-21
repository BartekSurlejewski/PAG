#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"

#pragma once
class Core
{
public:
	Core(Window*, Camera*);
	~Core();
	void update(GLuint, Texture[], Transform*);

private:
	void processInput(GLFWwindow*);
	void processMouseMovement();

	GLdouble xpos, ypos;
	GLfloat lastX = 500, lastY = 320;
	Window* screen;
	Camera* camera;
};

