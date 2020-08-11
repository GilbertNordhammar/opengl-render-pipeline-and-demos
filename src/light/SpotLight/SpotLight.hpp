#pragma once
#include "../attributes/PositionalLight.hpp"
#include "../attributes/PhongLight.hpp";
#include <glm/glm.hpp>

struct SpotLight {
	PhongLight phong;
	PositionalLight positional;
	float innerCuttoff;
	float outerCuttoff;
	glm::vec3 direction;
};