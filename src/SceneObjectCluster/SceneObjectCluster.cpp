#include "SceneObjectCluster.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <optick.h>

SceneObjectCluster::SceneObjectCluster(const SceneObjectCluster& other)
	: mModel(other.mModel),
	mShader(other.mShader),
	mTransforms(other.mTransforms),
	mModelMatrices(other.mModelMatrices),
	mUseInstancing(other.mUseInstancing)
{
	if (mUseInstancing)
		ApplyInstancedMatrices(true);
}

SceneObjectCluster::SceneObjectCluster(SceneObjectCluster&& other)
	: mModel(std::move(other.mModel)),
	mShader(std::move(other.mShader)),
	mTransforms(std::move(other.mTransforms)),
	mModelMatrices(std::move(other.mModelMatrices)),
	mUseInstancing(std::move(other.mUseInstancing))
{
	if (mUseInstancing)
		ApplyInstancedMatrices(true);
}

SceneObjectCluster::SceneObjectCluster(
	Model&& model,
	std::shared_ptr<Shader> shader,
	bool useInstancing)
	: mModel(model), mShader(shader), mUseInstancing(useInstancing) {}

SceneObjectCluster& SceneObjectCluster::operator=(SceneObjectCluster other) {
	Swap(*this, other);

	return *this;
}

void SceneObjectCluster::Swap(SceneObjectCluster& first, SceneObjectCluster& second) {
	using std::swap;

	swap(first.mModel, second.mModel);
	swap(first.mShader, second.mShader);
	swap(first.mTransforms, second.mTransforms);
	swap(first.mModelMatrices, second.mModelMatrices);
	swap(first.mUseInstancing, second.mUseInstancing);
}

void SceneObjectCluster::Update() {
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

void SceneObjectCluster::ApplyInstancedMatrices(bool createNewBuffer) {
	if (createNewBuffer) {
		for (int i = 0; i < mModel.mMeshes.size(); i++)
			mModel.mMeshes[i].SetInstancedModelMatrices(mModelMatrices);
	}
	else {
		for (int i = 0; i < mModel.mMeshes.size(); i++)
			mModel.mMeshes[i].UpdateInstancedModelMatrices(mModelMatrices, 0);
	}
}

void SceneObjectCluster::RecalculateMatrices() {
	int count = mModelMatrices.size();
	for (int i = 0; i < count; i++) {
		mModelMatrices[i] = mTransforms[i].CalculateModelMatrix();
	}
}

void SceneObjectCluster::Draw() {
	OPTICK_EVENT();

	mShader->Use();
	mShader->SetBool("_instancingEnabled", mUseInstancing);

	if (mUseInstancing) {
		DrawInstanced();
	}
	else {
		DrawSingle();
	}
}

void SceneObjectCluster::DrawSingle() {
	OPTICK_EVENT();

	for (const auto& mesh : mModel.mMeshes) {
		mesh.PassTextures(*mShader);
		for (const auto& modelMat : mModelMatrices) {
			mShader->SetMat4("_modelMatrix", modelMat);
			mesh.Draw();
		}
	}
}

void SceneObjectCluster::DrawInstanced() {
	OPTICK_EVENT();

	for (const auto& mesh : mModel.mMeshes) {
		mesh.PassTextures(*mShader);
		mesh.DrawInstanced(mModelMatrices.size());
	}
}