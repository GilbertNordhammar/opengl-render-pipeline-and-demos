#pragma once
#include <glm/glm.hpp>

struct PositionalLight {
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
};