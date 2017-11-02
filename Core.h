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
	void update(GLuint, Shader, Texture, Transform);

private:
	void render(Shader, Texture);
	void processInput(GLFWwindow*);

	Window* screen;
	Camera* camera;
};

