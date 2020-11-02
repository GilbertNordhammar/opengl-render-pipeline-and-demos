#include "Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

glm::mat4 Transform::CalculateModelMatrix() const {
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, mPosition);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(mRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(mRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(mRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, mScale);

	return modelMatrix;
}