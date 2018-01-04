#include "stdafx.h"
#include "GraphNode.h"


GraphNode::GraphNode() : local(Transform::origin())
{
}

void GraphNode::render(GLuint programHandle, Transform parentWorld, Shader* shader)
{

	local.CalculateWorldMatrix();
	world = local.combine(parentWorld);

	for each(Mesh mesh in meshes)
	{
		
		renderMesh(programHandle, &mesh, world, shader);
		transformLoc = glGetUniformLocation(shader->programHandle, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(world.transformation));
	}

	for (int i = 0; i < numChildren; i++)
	{
		children[i]->render(programHandle, world, shader);
	}
}

void GraphNode::SetAABB(const glm::vec3& vMin, const glm::vec3& vMax)
{
	AABBMin = vMin;
	AABBMax = vMax;
}

void GraphNode::GetAABB(glm::vec3& vMin, glm::vec3& vMax)
{
	vMin = AABBMin;
	vMax = AABBMax;
}

void GraphNode::setTransform(Transform local)
{
	this->local = local;
}

void GraphNode::renderMesh(GLuint programHandle, Mesh *mesh, Transform transform, Shader* shader)
{
	mesh->Draw(shader);
}

void GraphNode::addChild(GraphNode* node)
{
	children.push_back(node);
	++numChildren;
}
