#include "SceneObject.hpp"
#include <optick.h>

SceneObject::SceneObject(
	std::shared_ptr<Model> model,
	std::shared_ptr<Shader> shader,
	Transform transform)
	: mModel(model), mShader(shader), mTransform(transform) {}

void SceneObject::Draw() const {
	OPTICK_EVENT();

	mShader->Use();
	mShader->SetMat4("model", mTransform.CalculateModelMatrix());
	for (const auto& mesh : mModel->mMeshes) {
		mesh.PassTextures(*mShader);
		mesh.Draw();
	}
}

