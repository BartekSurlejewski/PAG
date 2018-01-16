#include "ModelNode.h"


ModelNode::ModelNode()
{
	boundingBoxMin = glm::vec3(FLT_MAX);
	boundingBoxMax = glm::vec3(FLT_MIN);
	color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	nodeType = Type_Model;
}

void ModelNode::Draw(const Transform & parentTransform, Shader * shader)
{
	shader->UseProgram();
	xForm.CalculateWorldMatrix();
	combinedXForm = parentTransform.worldMatrix * xForm.worldMatrix;
	shader->SetMat4("model", combinedXForm);

	GLuint colorLocation = glGetUniformLocation(shader->ID, "color");
	glUniform4f(colorLocation, color.x, color.y, color.z, color.w);

	for (int i = 0; i < meshesID.size(); i++)
	{
		const unsigned int meshID = meshesID[i];
		std::vector<Mesh>& meshesVector = *meshes;
		meshesVector[meshID].Draw(*shader);
	}
	xForm.worldMatrix = combinedXForm;

	for (int i = 0; i < children.size(); i++) //mo¿e size -1
	{
		children[i]->Draw(xForm, shader);
	}
}
