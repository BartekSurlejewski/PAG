#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#pragma once
class Texture
{
public:
	Texture();
	~Texture();

	unsigned int id;
	std::string type;
	std::string path;
};

