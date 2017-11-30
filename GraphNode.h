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

	vector<Mesh> meshes;
	std::vector<GraphNode*> children;
	Transform local;

private:
	Transform world;
	int numChildren = 0;
	GLuint transformLoc;

	void setTransform(Transform);
	void renderMesh(GLuint, Mesh*, Transform, Shader*);
};
