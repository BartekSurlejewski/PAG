#include <glad/glad.h>
#include <glm/glm.hpp>

#pragma once
class Mesh
{
public:
	Mesh();
	~Mesh();
	int loadContent(int, GLuint);
};

