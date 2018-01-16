#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	: SceneNode()
{
	nodeType = Type_DirectionalLight;
	m_direction = glm::vec3(0.f, -1.f, 0.f);
}

DirectionalLight::~DirectionalLight()
{

}

//------------------------------------------------------------------------
void DirectionalLight::Draw(const Transform& parentTransform, Shader* pShader)
{
	pShader->UseProgram();

	pShader->SetVec3("dirLight.ambient", m_material.ambient);
	pShader->SetVec3("dirLight.diffuse", m_material.diffuse);
	pShader->SetVec3("dirLight.specular", m_material.specular);
	pShader->SetVec3("dirLight.direction", m_direction);

	SceneNode::Draw(parentTransform, pShader);
}

//------------------------------------------------------------------------
void DirectionalLight::SetDirection(const glm::vec3& v)
{
	m_direction = v;
}

//------------------------------------------------------------------------
void DirectionalLight::SetMaterial(const Material& mat)
{
	m_material = mat;
}