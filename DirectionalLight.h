#pragma once
#include "Node.h"
#include "Material.h"

class DirectionalLight : public SceneNode
{
public:
	DirectionalLight();
	virtual ~DirectionalLight();

	virtual void Draw(const Transform& parentTransform, Shader* pShader);

	glm::vec3& GetDirection() { return m_direction; }
	void SetDirection(const glm::vec3& v);

	void SetMaterial(const Material& mat);

private:
	Material m_material;
	glm::vec3 m_direction;
};