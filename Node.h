#pragma once
#include "Mesh.h"
#include "Transform.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum NodeType
{
	Type_Node = -1,
	Type_Model,
	Type_PointLight,
	Type_DirectionalLight,
	Type_SpotLight,
};

struct SceneNode
{

	SceneNode()
	{
		nodeType = Type_Node;
	}
	virtual ~SceneNode() {}

	void SetName(const std::string& newName) { name = newName; }
	std::string GetName() const { return name; }

	virtual void Draw(const Transform& parentTransform, Shader* parentShader) {};

	// each node has its own xform
	// Local transform
	Transform xForm;

	// Combined transform
	glm::mat4 combinedXForm;

	// Name of this node
	std::string name;

	NodeType nodeType;

	// Children
	std::vector<SceneNode*> children;
};

struct SceneGraph
{
	SceneNode* rootNode;
};