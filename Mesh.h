#pragma once
#include "Shader.h"

#include <glm\glm.hpp>
#include <vector>

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
};

struct MeshTexture
{
	unsigned int id;
	std::string type;
};

class Mesh
{
public:
	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);

	void SetBoundingBox(glm::vec3& min, glm::vec3& max);
	void GetBoundingBox(glm::vec3& min, glm::vec3& max);

	void Draw(Shader shader);

	glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	unsigned int GetVBO() const { return VBO; }
	unsigned int GetVAO() const { return VAO; }
	unsigned int GetEBO() const { return EBO; }

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<MeshTexture> textures;

	glm::vec3 boudingBoxMin;
	glm::vec3 boudingBoxMax;

	unsigned int VBO;
	unsigned int EBO;
	unsigned int VAO;
	void SetupMesh();
};
