#pragma once

#include "src/Model/Model.hpp"
#include "src/Shader/Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class SceneObject {
public:
	std::shared_ptr<Model> mModel;
	std::shared_ptr <Shader> mShader;
	glm::vec3 mPosition;
	glm::vec3 mRotation;
	glm::vec3 mScale;

	SceneObject(
		std::shared_ptr<Model> model,
		std::shared_ptr <Shader> shader,
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f))
		: mModel(model), 
		mShader(shader), 
		mPosition(position), 
		mRotation(rotation), 
		mScale(scale) {}

	void Draw() {
		mShader->Use();
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, mPosition);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(mRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(mRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(mRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, mScale);
		mShader->SetMat4("model", modelMatrix);

		mModel->Draw(*mShader);
	}
};