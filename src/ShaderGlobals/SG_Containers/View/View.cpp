#include "View.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

SG_Containers::View::View(GLuint bindingPoint, std::string uniformBlockName)
	: mBindingPoint(bindingPoint), mUniformBlockName(uniformBlockName)
{
	BindBuffer();
};

void SG_Containers::View::BindBlock(GLuint shaderObj) {
	GLint matricesBlock = glGetUniformBlockIndex(shaderObj, mUniformBlockName.c_str());
	glUniformBlockBinding(shaderObj, matricesBlock, mBindingPoint);
}

void SG_Containers::View::BindBuffer() {
	glBindBuffer(GL_UNIFORM_BUFFER, mBuffer.GetFirst());
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, mBindingPoint, mBuffer.GetFirst());
}

void SG_Containers::View::SetProjectionMatrix(const glm::mat4& matrix) {
	mProjectionMatrix = matrix;

	glBindBuffer(GL_UNIFORM_BUFFER, mBuffer.GetFirst());
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(matrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SG_Containers::View::SetViewMatrix(const glm::mat4& matrix) {
	mViewMatrix = matrix;

	glBindBuffer(GL_UNIFORM_BUFFER, mBuffer.GetFirst());
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(matrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SG_Containers::View::SetViewPosWS(const glm::vec3& worldPos) {
	mViewPosWS = worldPos;

	glm::vec4 pos = glm::vec4(worldPos.x, worldPos.y, worldPos.z, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, mBuffer.GetFirst());
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec4), glm::value_ptr(pos));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}