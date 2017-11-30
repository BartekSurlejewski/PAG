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
	GraphNode(Mesh*);	
	~GraphNode();
	void render(GLuint, Transform, bool, Shader);
	void addChild(GraphNode node);

	vector<Mesh> meshes;

private:
	Transform local;
	Transform world;
	Mesh* mesh;
	std::vector<GraphNode*> children;
	int numChildren = 0;
	bool dirty;

	void setTransform(Transform);
	void renderMesh(GLuint, Mesh*, Transform, Shader);
};
