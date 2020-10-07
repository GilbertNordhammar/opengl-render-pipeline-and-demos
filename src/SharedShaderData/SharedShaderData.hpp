#pragma once
#include <glm/glm.hpp>
#include "src/OpenGLObjectGenerator/OpenGLObjectGenerator.hpp"

class SharedShaderData
{
public:
	static SharedShaderData& Get() {
		static SharedShaderData instance;
		return instance;
	}

	void BindBlocks(GLuint shaderObj);

	void SetViewMatrix(const glm::mat4& matrix);
	void SetProjectionMatrix(const glm::mat4& matrix);
	
	const glm::mat4& GetViewMatrix() const { return mViewMatrix; }
	const glm::mat4& GetProjectionMatrix() const { return mProjectionMatrix; }
private:
	OpenGLObjectArray mMatrices = OpenGLObjectGenerator::GenBuffers(1);

	glm::mat4 mViewMatrix;
	glm::mat4 mProjectionMatrix;

	SharedShaderData();
	void BindMatrices();
};

