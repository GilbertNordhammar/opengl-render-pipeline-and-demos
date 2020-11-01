#pragma once

#include "src/Model/Model.hpp"
#include "src/Shader/Shader.hpp"
#include <glm/glm.hpp>
#include <vector>
#include "src/Transform/Transform.hpp"

class SceneObject {
public:
	Model mModel;
	std::shared_ptr<Shader> mShader;
	std::vector<Transform> mTransforms;

	SceneObject(const SceneObject& other);
	SceneObject(SceneObject&& other);
	SceneObject(
		Model&& model,
		std::shared_ptr<Shader> shader,
		bool useInstancing);

	SceneObject& operator=(SceneObject other);

	void Update();
	void Draw();
private:
	std::vector<glm::mat4> mModelMatrices;
	bool mUseInstancing;

	void ApplyInstancedMatrices(bool createNewBuffer);
	void RecalculateMatrices();
	void DrawSingle();
	void DrawInstanced();
	void Swap(SceneObject& first, SceneObject& second);
};
