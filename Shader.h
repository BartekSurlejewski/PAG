#pragma once

#include <string>
#include <glm\glm.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
public:
	Shader();
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	void UseProgram();

	unsigned int GetID() const;
	void SetID(unsigned int id);
	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
	void SetMat4(const std::string &name, const glm::mat4 &mat) const;
	void SetVec3(const std::string &name, const glm::vec3 &value) const;
private:

	void CheckCompileErrors(unsigned shader, std::string type);

};