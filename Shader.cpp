#include "stdafx.h"
#include "Shader.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>


Shader::Shader(GLuint programHandle, std::string vertexShader, std::string fragmentShader)
{
	if (programHandle == 0)
	{
		std::cout << "Error creating program object" << std::endl;
	}

	loadAndCompileShaderFromFile(GL_VERTEX_SHADER, vertexShader, programHandle);
	loadAndCompileShaderFromFile(GL_FRAGMENT_SHADER, fragmentShader, programHandle);
	glLinkProgram(programHandle);

	glUseProgram(programHandle);
}


Shader::~Shader()
{
}

std::string Shader::loadShader(std::string fileName)
{
	std::string fileText;
	std::string line;
	std::ifstream inFile(fileName);

	if (!inFile)
	{
		std::cout << "Could not open file " << fileName.c_str() << std::endl;
		inFile.close();

		return NULL;
	}
	else
	{
		while (inFile.good())
		{
			getline(inFile, line);
			fileText.append(line + "\n");
		}

		inFile.close();

		return fileText;
	}
}

void Shader::loadAndCompileShaderFromFile(GLint shaderType, std::string fileName, GLuint& programHandle)
{
	shaderObject = glCreateShader(shaderType);

	if (shaderObject == 0) 
	{
		std::cout << "Error creating " << fileName.c_str() << std::endl;
		return;
	}

	std::string shaderCodeString = loadShader(fileName);

	if (shaderCodeString.empty())
	{
		std::cout << "Shader code is empty. Shader name: " << fileName.c_str() << std::endl;
		return;
	}

	const char* shaderCode = shaderCodeString.c_str();
	const GLint codeSize = shaderCodeString.size();

	glShaderSource(shaderObject, 1, &shaderCode, &codeSize);

	glCompileShader(shaderObject);

	GLint result;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		std::cout << fileName.c_str() << " compilation failed" << std::endl;

		GLint logLength;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			char* log = (char*) malloc (logLength);

			GLsizei written;
			glGetShaderInfoLog(shaderObject, logLength, &written, log);

			std::cout << "Shader log:\n" << log << std::endl;
			free(log);
		}

		return;
	}

	glAttachShader(programHandle, shaderObject);
	glDeleteShader(shaderObject);
}