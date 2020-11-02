#pragma once
#include "src/Transform/Transform.hpp"
#include "src/Shader/Shader.hpp"
#include "src/Model/Model.hpp"
#include <memory>

class SceneObject
{
public:
	std::shared_ptr<Model> mModel;
	std::shared_ptr<Shader> mShader;
	Transform mTransform;

	SceneObject(
		std::shared_ptr<Model> model,
		std::shared_ptr<Shader> shader,
		Transform transform);

	void Draw() const;
};

