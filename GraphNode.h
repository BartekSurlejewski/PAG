#pragma once
#include"Transform.h"
#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <vector>

class GraphNode
{
public:
	GraphNode();	
	void render(GLuint, Transform, Shader*);
	void addChild(GraphNode* node);
	void SetAABB(const glm::vec3& vMin, const glm::vec3& vMax);
	void GetAABB(glm::vec3& vMin, glm::vec3& vMax);

	vector<Mesh> meshes;
	vector<unsigned int> meshesIds;

	std::vector<GraphNode*> children;
	Transform local;
	glm::vec3 AABBMin;
	glm::vec3 AABBMax;

	glm::vec3 boundingSphereCenter;
	float boundingSphereRadius;

private:
	Transform world;
	int numChildren = 0;
	GLuint transformLoc;

	void setTransform(Transform);
	void renderMesh(GLuint, Mesh*, Transform, Shader*);
};
