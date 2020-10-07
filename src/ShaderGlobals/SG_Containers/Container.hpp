#pragma once

#include <glad/glad.h>
namespace SG_Containers {
	class Container {
	public:
		virtual void BindBlock(GLuint shaderObj) = 0;
	private:
		virtual void BindBuffer() = 0;
	};
}