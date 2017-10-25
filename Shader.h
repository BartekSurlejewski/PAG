#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

#pragma once
class Shader
{
public:
	Shader();
	~Shader();
	void loadAndCompileShaderFromFile(GLint, std::string, GLuint&);

private:
	std::string loadShader(std::string);
};

