#include "SharedShaderData.hpp"
#include <glad/glad.h>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef unsigned int BindingPoint;

static BindingPoint BP_MATRICES = 0;

SharedShaderData::SharedShaderData() {
	BindMatrices();
}

void SharedShaderData::BindBlocks(GLuint shaderObj) {
	GLuint matricesBlock = glGetUniformBlockIndex(shaderObj, "rp_Matrices");
	glUniformBlockBinding(shaderObj, matricesBlock, BP_MATRICES);
}

void SharedShaderData::BindMatrices() {
	glBindBuffer(GL_UNIFORM_BUFFER, mMatrices.GetFirst());
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, BP_MATRICES, mMatrices.GetFirst());
}

void SharedShaderData::SetProjectionMatrix(const glm::mat4& matrix) {
	glBindBuffer(GL_UNIFORM_BUFFER, mMatrices.GetFirst());
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(matrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedShaderData::SetViewMatrix(const glm::mat4& matrix) {
	glBindBuffer(GL_UNIFORM_BUFFER, mMatrices.GetFirst());
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(matrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}