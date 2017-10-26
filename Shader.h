#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

#pragma once
class Shader
{
public:
	Shader(GLuint, std::string, std::string);
	~Shader();

private:
	std::string loadShader(std::string);
	void loadAndCompileShaderFromFile(GLint, std::string, GLuint&);
};

