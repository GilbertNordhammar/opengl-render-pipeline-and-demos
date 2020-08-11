#pragma once
#include "../attributes/PositionalLight.hpp"
#include "../attributes/PhongLight.hpp";

struct PointLight {
	PhongLight phong;
	PositionalLight positional;
};