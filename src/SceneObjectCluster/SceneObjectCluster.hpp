#pragma once

#include "src/Model/Model.hpp"
#include "src/Shader/Shader.hpp"
#include <glm/glm.hpp>
#include <vector>
#include "src/Transform/Transform.hpp"

class SceneObjectCluster {
public:
	Model mModel;
	std::shared_ptr<Shader> mShader;
	std::vector<Transform> mTransforms;

	SceneObjectCluster(const SceneObjectCluster& other);
	SceneObjectCluster(SceneObjectCluster&& other);
	SceneObjectCluster(
		Model&& model,
		std::shared_ptr<Shader> shader,
		bool useInstancing);

	SceneObjectCluster& operator=(SceneObjectCluster other);

	void Update();
	void Draw();
private:
	std::vector<glm::mat4> mModelMatrices;
	bool mUseInstancing;

	void ApplyInstancedMatrices(bool createNewBuffer);
	void RecalculateMatrices();
	void DrawSingle();
	void DrawInstanced();
	void Swap(SceneObjectCluster& first, SceneObjectCluster& second);
};
