#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#pragma once
class Texture
{
public:
	Texture();
	~Texture();
	void loadTexture(std::string);
};

