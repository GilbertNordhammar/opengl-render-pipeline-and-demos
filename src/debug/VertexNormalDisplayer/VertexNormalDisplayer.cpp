#include "VertexNormalDisplayer.hpp"
#include "src/utils/fileUtils.h"
#include <string>

using namespace fileUtils;

VertexNormalDisplayer::VertexNormalDisplayer() :
    mVertexNormalShader(
        getFullResourcesPath("shaders/debug/DisplayVertexNormals/DisplayVertexNormals.vert"),
        getFullResourcesPath("shaders/debug/DisplayVertexNormals/DisplayVertexNormals.frag"),
        getFullResourcesPath("shaders/debug/DisplayVertexNormals/DisplayVertexNormals.geom")) {}

void VertexNormalDisplayer::Draw(const SceneObject& sceneObject) {
    sceneObject.Draw();
    mVertexNormalShader.Use();

    mVertexNormalShader.SetMat4("matrix_M", sceneObject.mTransform.CalculateModelMatrix());
    for (const auto& mesh : sceneObject.mModel->mMeshes) {
        mesh.PassTextures(mVertexNormalShader);
        mesh.Draw();
    }
}
