#pragma once
#include <glm/glm.hpp>
#include "src/OpenGLObjectGenerator/OpenGLObjectGenerator.hpp"
#include <string>
#include "../Container.hpp"

namespace SG_Containers {
	class View : public Container {
	public:
		View(GLuint bindingPoint, std::string uniformBlockName);

		void BindBlock(GLuint shaderObj);

		void SetViewMatrix(const glm::mat4& matrix);
		void SetProjectionMatrix(const glm::mat4& matrix);
		void SetViewPosWS(const glm::vec3& worldPos);

		const glm::mat4& GetViewMatrix() const { return mViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return mProjectionMatrix; }
		const glm::vec3& GetViewPosWS() const { return mViewPosWS; }
	private:
		void BindBuffer();

		const GLuint mBindingPoint;
		const std::string mUniformBlockName;
		OpenGLObjectArray mBuffer = OpenGLObjectGenerator::GenBuffers(1);

		glm::mat4 mViewMatrix;
		glm::mat4 mProjectionMatrix;
		glm::vec3 mViewPosWS;
	};
}

