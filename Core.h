#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "Shader.h"
#include "GraphNode.h"
#include "Model.h"
#include "Scene.h"

#pragma once
class Core
{
public:
	Core(Window*, Camera*, Shader);
	~Core();
	void update(GLuint, Shader);

private:
	void processInput(GLFWwindow*);
	//void processMouseMovement();

	GLdouble xpos, ypos;
	GLfloat lastX = 500, lastY = 320;
	Window* screen;
	Camera* camera;
	Scene* scene;
};

