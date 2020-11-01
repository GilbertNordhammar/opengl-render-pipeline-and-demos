#include "SceneObject.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <optick.h>

SceneObject::SceneObject(const SceneObject& other)
	: mModel(other.mModel),
	mShader(other.mShader),
	mTransforms(other.mTransforms),
	mModelMatrices(other.mModelMatrices),
	mUseInstancing(other.mUseInstancing)
{
	if (mUseInstancing)
		ApplyInstancedMatrices(true);
}

SceneObject::SceneObject(SceneObject&& other)
	: mModel(std::move(other.mModel)),
	mShader(std::move(other.mShader)),
	mTransforms(std::move(other.mTransforms)),
	mModelMatrices(std::move(other.mModelMatrices)),
	mUseInstancing(std::move(other.mUseInstancing))
{
	if (mUseInstancing)
		ApplyInstancedMatrices(true);
}

SceneObject::SceneObject(
	Model&& model,
	std::shared_ptr<Shader> shader,
	bool useInstancing)
	: mModel(model), mShader(shader), mUseInstancing(useInstancing) {}

SceneObject& SceneObject::operator=(SceneObject other) {
	Swap(*this, other);

	return *this;
}

void SceneObject::Swap(SceneObject& first, SceneObject& second) {
	using std::swap;

	swap(first.mModel, second.mModel);
	swap(first.mShader, second.mShader);
	swap(first.mTransforms, second.mTransforms);
	swap(first.mModelMatrices, second.mModelMatrices);
	swap(first.mUseInstancing, second.mUseInstancing);
}

void SceneObject::Update() {
	bool hasResized = false;
	int count = mTransforms.size();
	if (mModelMatrices.size() != count) {
		mModelMatrices.resize(count);
		hasResized = true;
	}

	RecalculateMatrices();

	if (mUseInstancing)
		ApplyInstancedMatrices(hasResized);
}

void SceneObject::ApplyInstancedMatrices(bool createNewBuffer) {
	if (createNewBuffer) {
		for (int i = 0; i < mModel.mMeshes.size(); i++)
			mModel.mMeshes[i].SetInstancedModelMatrices(mModelMatrices);
	}
	else {
		for (int i = 0; i < mModel.mMeshes.size(); i++)
			mModel.mMeshes[i].UpdateInstancedModelMatrices(mModelMatrices, 0);
	}
}

void SceneObject::RecalculateMatrices() {
	int count = mModelMatrices.size();
	for (int i = 0; i < count; i++) {
		mModelMatrices[i] = glm::mat4(1.0f);
		mModelMatrices[i] = glm::translate(mModelMatrices[i], mTransforms[i].mPosition);
		mModelMatrices[i] = glm::rotate(mModelMatrices[i], glm::radians(mTransforms[i].mRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		mModelMatrices[i] = glm::rotate(mModelMatrices[i], glm::radians(mTransforms[i].mRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		mModelMatrices[i] = glm::rotate(mModelMatrices[i], glm::radians(mTransforms[i].mRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		mModelMatrices[i] = glm::scale(mModelMatrices[i], mTransforms[i].mScale);
	}
}

void SceneObject::Draw() {
	OPTICK_EVENT();

	mShader->Use();
	mShader->SetBool("rp_instancingEnabled", mUseInstancing);

	if (mUseInstancing) {
		DrawInstanced();
	}
	else {
		DrawSingle();
	}
}

void SceneObject::DrawSingle() {
	OPTICK_EVENT();

	for (const auto& mesh : mModel.mMeshes) {
		mesh.PassTextures(*mShader);
		for (const auto& modelMat : mModelMatrices) {
			mShader->SetMat4("model", modelMat);
			mesh.Draw();
		}
	}
}

void SceneObject::DrawInstanced() {
	OPTICK_EVENT();

	for (const auto& mesh : mModel.mMeshes) {
		mesh.PassTextures(*mShader);
		mesh.DrawInstanced(mModelMatrices.size());
	}
}