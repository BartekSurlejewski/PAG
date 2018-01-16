#include "SpotLight.h"

SpotLight::SpotLight() : SceneNode()
{
	nodeType = Type_SpotLight;
	m_position = glm::vec3(0.0f, 0.0f, 40.0f);
	m_cutoff = glm::cos(glm::radians(45.0f));
	m_att = glm::vec3(1.0f, 0.02f, 0.0001f);
}

SpotLight::~SpotLight()
{

}

void SpotLight::Draw(const Transform& parentTransform, Shader* pShader)
{
	pShader->UseProgram();

	m_direction = -m_position;			// (0,0,0) - (m_position)

	pShader->SetVec3("spotLight.ambient", m_material.ambient);
	pShader->SetVec3("spotLight.diffuse", m_material.diffuse);
	pShader->SetVec3("spotLight.specular", m_material.specular);
	pShader->SetVec3("spotLight.att", m_att);
	pShader->SetVec3("spotLight.posWorld", m_position);
	pShader->SetVec3("spotLight.direction", m_direction);
	pShader->SetFloat("spotLight.cutoff", m_cutoff);

	SceneNode::Draw(parentTransform, pShader);
}

//------------------------------------------------------------------------
glm::vec3& SpotLight::GetPosition()
{
	return m_position;
}

glm::vec3& SpotLight::GetDirection()
{
	return m_direction;
}

float& SpotLight::GetCutoff()
{
	return m_cutoff;
}

void SpotLight::SetMaterial(const Material& mat)
{
	m_material = mat;
}

void SpotLight::SetPosition(const glm::vec3& p)
{
	m_position = p;
}

