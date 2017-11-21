#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#pragma once
class Texture
{
public:
	Texture(std::string);
	~Texture();
	void loadTexture(std::string);
	GLuint getTexture();

private:
	GLuint texture;
};

