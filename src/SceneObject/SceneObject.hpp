#pragma once

#include "src/Model/Model.hpp"
#include "src/Shader/Shader.hpp"
#include <glm/glm.hpp>
#include <vector>
#include "src/Transform/Transform.hpp"

class SceneObject {
public:
	std::shared_ptr<Model> mModel;
	std::shared_ptr<Shader> mShader;

	std::vector<Transform> mTransforms;

	SceneObject(
		std::shared_ptr<Model> model,
		std::shared_ptr<Shader> shader,
		bool useInstancing)
		: mModel(model), mShader(shader), mUseInstancing(useInstancing) {}

	void Update();
	void Draw();
private:
	std::vector<glm::mat4> mModelMatrices;
	bool mUseInstancing;

	void ApplyInstancedMatrices(bool hasResized);
	void RecalculateMatrices();
	void DrawSingle();
	void DrawInstanced();
};
