#include "PointLight.h"

PointLight::PointLight() : SceneNode()
{
	nodeType = Type_PointLight;
	m_att = glm::vec3(0.2f, 0.001f, 0.0001f);
}

PointLight::~PointLight()
{

}

void PointLight::Draw(const Transform& parentTransform, Shader* pShader)
{
	pShader->UseProgram();

	pShader->SetVec3("pointLight.ambient", m_material.ambient);
	pShader->SetVec3("pointLight.diffuse", m_material.diffuse);
	pShader->SetVec3("pointLight.specular", m_material.specular);
	pShader->SetVec3("pointLight.posWorld", m_pos);
	pShader->SetVec3("pointLight.att", m_att);

	SceneNode::Draw(parentTransform, pShader);
}

//------------------------------------------------------------------------
glm::vec3& PointLight::GetPosition()
{
	return m_pos;
}

//------------------------------------------------------------------------
void PointLight::SetPosition(const glm::vec3& v)
{
	m_pos = v;
}

//------------------------------------------------------------------------
glm::vec3& PointLight::GetAttenuation()
{
	return m_att;
}

void PointLight::SetMaterial(const Material& mat)
{
	m_material = mat;
}