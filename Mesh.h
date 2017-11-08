#pragma once
class Mesh
{
public:
	Mesh();
	~Mesh();
	int loadContent(int);
	static glm::vec3 cubePositions[];
};

