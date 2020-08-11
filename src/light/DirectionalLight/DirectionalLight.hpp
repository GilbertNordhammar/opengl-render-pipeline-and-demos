#pragma once
#include "../attributes/PhongLight.hpp"
#include <glm/glm.hpp>

struct DirectionalLight {
	PhongLight phong;
	glm::vec3 direction;
};