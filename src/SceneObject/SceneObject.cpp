#include "SceneObject.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <optick.h>

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

void SceneObject::ApplyInstancedMatrices(bool hasResized) {
	if (hasResized) {
		for (int i = 0; i < mModel->mMeshes.size(); i++)
			mModel->mMeshes[i].SetInstancedModelMatrices(mModelMatrices);
	}
	else {
		for (int i = 0; i < mModel->mMeshes.size(); i++)
			mModel->mMeshes[i].UpdateInstancedModelMatrices(mModelMatrices, 0);
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

	for (const auto& mesh : mModel->mMeshes) {
		mesh.PassTextures(*mShader);
		for (const auto& modelMat : mModelMatrices) {
			mShader->SetMat4("model", modelMat);
			mesh.Draw();
		}
	}
}

void SceneObject::DrawInstanced() {
	OPTICK_EVENT();

	for (const auto& mesh : mModel->mMeshes) {
		mesh.PassTextures(*mShader);
		mesh.DrawInstanced(mModelMatrices.size());
	}
}