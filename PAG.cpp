#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Core.h"

#define STB_IMAGE_IMPLEMENTATION

const int SCR_WIDTH = 640;
const int SCR_HEIGHT = 480;

GLuint VBO = NULL;

int main()
{
	Window* screen = new Window();
	Mesh mesh;
	Core core(screen);

	if (!screen->init(SCR_WIDTH, SCR_HEIGHT)) 
	{
		return -1;
	}

	if (!mesh.loadContent(1, VBO))
	{
		return -1;
	}

	core.update();

	glfwTerminate();
	return 0;
}