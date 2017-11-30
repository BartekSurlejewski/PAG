//#include "stdafx.h"
//#include "Shader.h"
//#include <iostream>
//#include <glad/glad.h>
//#include <glm/glm.hpp>
//#include <GLFW/glfw3.h>
//#include <string>
//#include <fstream>
//
//
//Shader::Shader(GLuint programHandle, std::string vertexShader, std::string fragmentShader)
//{
//	if (programHandle == 0)
//	{
//		std::cout << "Error creating program object" << std::endl;
//	}
//
//	loadAndCompileShaderFromFile(GL_VERTEX_SHADER, vertexShader, programHandle);
//	loadAndCompileShaderFromFile(GL_FRAGMENT_SHADER, fragmentShader, programHandle);
//	glLinkProgram(programHandle);
//
//	glUseProgram(programHandle);
//}
//
//
//Shader::~Shader()
//{
//}
//
//std::string Shader::loadShader(std::string fileName)
//{
//	std::string fileText;
//	std::string line;
//	std::ifstream inFile(fileName);
//
//	if (!inFile)
//	{
//		std::cout << "Could not open file " << fileName.c_str() << std::endl;
//		inFile.close();
//
//		return NULL;
//	}
//	else
//	{
//		while (inFile.good())
//		{
//			getline(inFile, line);
//			fileText.append(line + "\n");
//		}
//
//		inFile.close();
//
//		return fileText;
//	}
//}
//
//void Shader::loadAndCompileShaderFromFile(GLint shaderType, std::string fileName, GLuint& programHandle)
//{
//	shaderObject = glCreateShader(shaderType);
//
//	if (shaderObject == 0) 
//	{
//		std::cout << "Error creating " << fileName.c_str() << std::endl;
//		return;
//	}
//
//	std::string shaderCodeString = loadShader(fileName);
//
//	if (shaderCodeString.empty())
//	{
//		std::cout << "Shader code is empty. Shader name: " << fileName.c_str() << std::endl;
//		return;
//	}
//
//	const char* shaderCode = shaderCodeString.c_str();
//	const GLint codeSize = shaderCodeString.size();
//
//	glShaderSource(shaderObject, 1, &shaderCode, &codeSize);
//
//	glCompileShader(shaderObject);
//
//	GLint result;
//	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);
//
//	if (result == GL_FALSE)
//	{
//		std::cout << fileName.c_str() << " compilation failed" << std::endl;
//
//		GLint logLength;
//		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logLength);
//
//		if (logLength > 0)
//		{
//			char* log = (char*) malloc (logLength);
//
//			GLsizei written;
//			glGetShaderInfoLog(shaderObject, logLength, &written, log);
//
//			std::cout << "Shader log:\n" << log << std::endl;
//			free(log);
//		}
//
//		return;
//	}
//
//	glAttachShader(programHandle, shaderObject);
//	glDeleteShader(shaderObject);
//}

//#include "Shader.h"
//
//Shader::Shader(GLuint ID, const char* vertexPath, const char* fragmentPath)
//{
//	this->ID = ID;
//	// 1. retrieve the vertex/fragment source code from filePath
//	std::string vertexCode;
//	std::string fragmentCode;
//	std::ifstream vShaderFile;
//	std::ifstream fShaderFile;
//	// ensure ifstream objects can throw exceptions:
//	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//	try
//	{
//		// open files
//		vShaderFile.open(vertexPath);
//		fShaderFile.open(fragmentPath);
//		std::stringstream vShaderStream, fShaderStream;
//		// read file's buffer contents into streams
//		vShaderStream << vShaderFile.rdbuf();
//		fShaderStream << fShaderFile.rdbuf();
//		// close file handlers
//		vShaderFile.close();
//		fShaderFile.close();
//		// convert stream into string
//		vertexCode = vShaderStream.str();
//		fragmentCode = fShaderStream.str();
//	}
//	catch (std::ifstream::failure e)
//	{
//		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
//	}
//	const char* vShaderCode = vertexCode.c_str();
//	const char * fShaderCode = fragmentCode.c_str();
//	// 2. compile shaders
//	unsigned int vertex, fragment;
//	int success;
//	char infoLog[512];
//	// vertex shader
//	vertex = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertex, 1, &vShaderCode, NULL);
//	glCompileShader(vertex);
//	checkCompileErrors(vertex, "VERTEX");
//	// fragment Shader
//	fragment = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragment, 1, &fShaderCode, NULL);
//	glCompileShader(fragment);
//	checkCompileErrors(fragment, "FRAGMENT");
//	// shader Program
//	//ID = glCreateProgram();
//	glAttachShader(ID, vertex);
//	glAttachShader(ID, fragment);
//	glLinkProgram(ID);
//	checkCompileErrors(ID, "PROGRAM");
//	// delete the shaders as they're linked into our program now and no longer necessary
//	glDeleteShader(vertex);
//	glDeleteShader(fragment);
//}
//
//void Shader::use()
//{
//	glUseProgram(ID);
//}
//
//void Shader::setBool(const std::string &name, bool value) const
//{
//	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
//}
//
//void Shader::setInt(const std::string &name, int value) const
//{
//	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
//}
//
//void Shader::setFloat(const std::string &name, float value) const
//{
//	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
//}
//
//void Shader::checkCompileErrors(unsigned int shader, std::string type)
//{
//	int success;
//	char infoLog[1024];
//	if (type != "PROGRAM")
//	{
//		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//		if (!success)
//		{
//			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
//			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
//		}
//	}
//	else
//	{
//		glGetProgramiv(shader, GL_LINK_STATUS, &success);
//		if (!success)
//		{
//			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
//			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
//		}
//	}
//}