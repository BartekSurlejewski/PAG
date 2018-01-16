#pragma once
#include "Mesh.h"

struct Material
{
	Material()
	{
		ambient = glm::vec3(1.f, 1.f, 1.f);
		diffuse = glm::vec3(1.f, 1.f, 1.f);
		specular = glm::vec3(1.f, 1.f, 1.f);
		specFactor = 1.f;
	}

	void SetSimple(Shader& shader)
	{
		shader.SetVec3("material.ambient", ambient);
		shader.SetVec3("material.diffuse", diffuse);
		shader.SetVec3("material.specular", specular);
		shader.SetFloat("material.specFactor", specFactor);
	}

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float specFactor;
};