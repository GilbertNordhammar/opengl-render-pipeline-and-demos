#pragma once
#include <glm/glm.hpp>

struct Transform {
	glm::vec3 mPosition;
	glm::vec3 mRotation;
	glm::vec3 mScale = glm::vec3(1, 1, 1);

	glm::mat4 CalculateModelMatrix() const;
};