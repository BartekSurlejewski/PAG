#pragma once
#include "Node.h"
#include "Material.h"

class PointLight : public SceneNode
{
public:
	PointLight();
	virtual ~PointLight();

	virtual void Draw(const Transform& parentTransform, Shader* pShader);

	glm::vec3& GetPosition();
	void SetPosition(const glm::vec3& v);

	glm::vec3& GetAttenuation();

	void SetMaterial(const Material& mat);

private:
	Material m_material;
	glm::vec3 m_pos;
	glm::vec3 m_att;

};