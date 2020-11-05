#pragma once
#include <memory>
#include "src/SceneObject/SceneObject.hpp"
#include "src/SceneObjectCluster/SceneObjectCluster.hpp"
#include "src/Shader/Shader.hpp"

class VertexNormalDisplayer
{
public:
	VertexNormalDisplayer();
	void Draw(const SceneObject& sceneObject);
private:
	Shader mVertexNormalShader;
};

