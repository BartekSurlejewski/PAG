#include "stdafx.h"
#include "GraphNode.h"


GraphNode::GraphNode(Mesh* mesh) : mesh(mesh),
local(Transform::origin()), dirty(true)
{
}


GraphNode::~GraphNode()
{
}

void GraphNode::render(GLuint programHandle, Transform parentWorld, bool dirty_, Shader shader)
{
	dirty_ |= dirty;
	if (dirty)
	{
		world = local.combine(parentWorld);
		dirty_ = false;
	}

	if (mesh) renderMesh(programHandle, mesh, world, shader);

	for (int i = 0; i < numChildren; i++)
	{
		children[i]->render(programHandle, world, dirty, shader);
	}
}

void GraphNode::setTransform(Transform local)
{
	this->local = local;
	dirty = true;
}

void GraphNode::renderMesh(GLuint programHandle, Mesh *mesh, Transform transform, Shader shader)
{
	GLuint transformLoc = glGetUniformLocation(programHandle, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform.transformation));
	mesh->Draw(&shader);
}

void GraphNode::addChild(GraphNode node)
{
	children.push_back(&node);
	++numChildren;
}
