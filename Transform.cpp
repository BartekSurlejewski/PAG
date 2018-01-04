#include "stdafx.h"
#include "Transform.h"

Transform Transform::origin()
{
	Transform transform;
	transform.transformation = glm::mat4(1.0f);
	return transform;
}

Transform Transform::combine(Transform& other)
{
	Transform transform;
	transform.transformation = other.transformation * this->transformation;

	return transform;
}

void Transform::CalculateWorldMatrix()
{
		transformation = glm::mat4(1.0f);
		transformation = glm::translate(transformation, translation);
		transformation = glm::rotate(transformation, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
		transformation = glm::rotate(transformation, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
		transformation = glm::rotate(transformation, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
		transformation = glm::scale(transformation, scale);
}