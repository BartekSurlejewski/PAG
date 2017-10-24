#include "Window.h"

#pragma once
class Core
{
public:
	Core(Window*);
	~Core();
	void update();

private:
	void render(float);
	void processInput(GLFWwindow*);

	Window* screen;

};

