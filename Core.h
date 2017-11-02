#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"

#pragma once
class Core
{
public:
	Core(Window*);
	~Core();
	void update(GLuint, Shader, Texture, Transform, Camera);

private:
	void render(Shader, Texture);
	void processInput(GLFWwindow*);

	Window* screen;
};

