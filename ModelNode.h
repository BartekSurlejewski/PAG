#pragma once

#include "Node.h"
#include <float.h>

class ModelNode : public SceneNode
{
public:
	ModelNode();
	virtual void Draw(const Transform& parentTransform, Shader* shader);

	glm::vec3 boundingBoxMin;
	glm::vec3 boundingBoxMax;

	glm::vec3 boundingSphereCenter;
	float boundingSphereRadius;

	glm::vec4 color;

	std::vector<unsigned int> meshesID;
	std::vector<Mesh>* meshes;
};