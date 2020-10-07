#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include "src/ShaderLoader/ShaderLoader.hpp"
#include "src/SharedShaderData/SharedShaderData.hpp"

Shader::Shader(
	const std::string& vertexPath,
	const std::string& fragmentPath,
	const std::string& geometryPath) 
	: mVertexPath(vertexPath), mFragmentPath(fragmentPath), mGeometryPath(geometryPath)
{
	mProgramID = ShaderLoader::Load(vertexPath, fragmentPath, geometryPath);
	SharedShaderData::Get().BindBlocks(mProgramID);
}

Shader::Shader(const Shader& other) 
	: Shader(other.mVertexPath, other.mFragmentPath, other.mGeometryPath) {}

Shader::Shader(Shader&& other) noexcept :
	mVertexPath(other.mVertexPath),
	mFragmentPath(other.mFragmentPath),
	mGeometryPath(other.mGeometryPath),
	mProgramID(other.mProgramID)
{
	other.mProgramID = 0;
}

Shader& Shader::operator=(Shader other) {
	Swap(*this, other);

	return *this;
}

void Shader::Swap(Shader& first, Shader& second) noexcept {
	using std::swap;

	swap(first.mProgramID, second.mProgramID);
	swap(first.mVertexPath, second.mVertexPath);
	swap(first.mFragmentPath, second.mFragmentPath);
	swap(first.mGeometryPath, second.mGeometryPath);
}

Shader::~Shader() {
	glDeleteProgram(mProgramID);
}

void Shader::Use() const
{
	glUseProgram(mProgramID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(mProgramID, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(mProgramID, name.c_str()), 1, &value[0]);
}
void Shader::SetVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(mProgramID, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, &value[0]);
}
void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(mProgramID, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, &value[0]);
}
void Shader::SetVec4(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(mProgramID, name.c_str()), x, y, z, w);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}