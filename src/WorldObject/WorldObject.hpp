#pragma once

#include "src/Model/Model.hpp"
#include "src/Shader/Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class WorldObject {
public:
	Model& mModel;
	Shader& mShader;
	glm::vec3 mPosition;
	glm::vec3 mRotation;
	glm::vec3 mScale;

	WorldObject(
		Model& model,
		Shader& shader,
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f))
		: mModel(model), 
		mShader(shader), 
		mPosition(position), 
		mRotation(rotation), 
		mScale(scale) {}

	void Draw() {
		mShader.use();
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, mPosition);
		modelMatrix = glm::scale(modelMatrix, mScale);
		mShader.setMat4("model", modelMatrix);
		mModel.Draw(mShader);
	}
};
