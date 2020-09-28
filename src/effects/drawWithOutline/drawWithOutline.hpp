#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "src/Shader/Shader.hpp"
#include "src/utils/array.h"
#include "src/SceneObject/SceneObject.hpp"
#include <vector>
#include <iostream>

// * outlineShader must include 'uniform mat4 model' in its vertex shader
// * thickness must be over 1.0f for visible effect

void drawWithOutline(
	std::vector<SceneObject> worldObjects,
	const Shader& outlineShader,
	float thickness,
	bool disableStencilTestAfterwards,
	bool enableDepthTestAfterwards = true,
	bool clearStencilBufferBefore = true
) {
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	if(clearStencilBufferBefore)
		glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	for (auto it = worldObjects.begin(); it != worldObjects.end(); ++it) {
		it->Draw();
	}

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	
	thickness = thickness > 1.0f ? thickness : 1.0f;
	for (auto it = worldObjects.begin(); it != worldObjects.end(); ++it) {
		auto origScale = it->mScale;
		it->mScale = glm::vec3(thickness);
		
		auto origShader = it->mShader;
		it->mShader = outlineShader;

		it->Draw();

		it->mScale = origScale;
		it->mShader = origShader;
	}

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);

	if(enableDepthTestAfterwards)
		glEnable(GL_DEPTH_TEST);

	if (disableStencilTestAfterwards)
		glDisable(GL_STENCIL_TEST);
}