#include "stdafx.h"
#include "Mesh.h"


Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

GLfloat vertices[] = {
						-0.5f, -0.5f, 0.0f, //Down left
						0.5f, -0.5f, 0.0f, //Down right
						0.0f, 0.5f, 0.0f //Upper
					};

//GLfloat vertices[] = {				//SQUARE
//				-0.75f, -0.75f, 0.0f, 
//				-0.75f, 0.75f, 0.0f, 
//				0.75f, -0.75f, 0.0f,
//
//				-0.75f, 0.75f, 0.0f, 
//				0.75f, 0.75f, 0.0f, 
//				0.75f, -0.75f, 0.0f 
//};

int Mesh::loadContent(int objectsCounter, GLuint VBO)
{
	glGenBuffers(objectsCounter, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

	return true;
}