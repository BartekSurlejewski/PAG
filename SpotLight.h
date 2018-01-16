#pragma once
#include "Node.h"
#include "Material.h"

class SpotLight : public SceneNode
{
public:
	SpotLight();
	virtual ~SpotLight();

	virtual void Draw(const Transform& parentTransform, Shader* pShader);

	glm::vec3& GetPosition();
	glm::vec3& GetDirection();
	glm::vec3& GetAttenuation() { return m_att; }
	float& GetCutoff();

	void SetMaterial(const Material& mat);
	void SetPosition(const glm::vec3& p);

private:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_att;
	float m_cutoff;
	Material m_material;

};