#include "stdafx.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm\gtx\transform.hpp>
#include "Mesh.h"

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

GLfloat vertices[] = {	//Positions			//Texture
						-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
						0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
						0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
						0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
						-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
						-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

						-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
						0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
						0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
						0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
						-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
						-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

						-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
						-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
						-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
						-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
						-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
						-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

						0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
						0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
						0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
						0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
						0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
						0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

						-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
						0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
						0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
						0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
						-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
						-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

						-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
						0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
						0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
						0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
						-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
						-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
							glm::vec3(0.0f,  0.0f,  0.0f),
							glm::vec3(2.0f,  5.0f, -15.0f),
							glm::vec3(-1.5f, -2.2f, -2.5f),
							glm::vec3(-3.8f, -2.0f, -12.3f),
							glm::vec3(2.4f, -0.4f, -3.5f),
							glm::vec3(-1.7f,  3.0f, -7.5f),
							glm::vec3(1.3f, -2.0f, -2.5f),
							glm::vec3(1.5f,  2.0f, -2.5f),
							glm::vec3(1.5f,  0.2f, -1.5f),
							glm::vec3(-1.3f,  1.0f, -1.5f)
};

int Mesh::loadContent(int objectsCounter)
{
	GLuint VBO = NULL;
	GLuint VAO = NULL;

	glGenBuffers(objectsCounter, &VBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	return true;
}