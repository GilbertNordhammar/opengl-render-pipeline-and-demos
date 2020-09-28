#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(
	const std::string vertexPath,
	const std::string fragmentPath,
	const std::string geometryPath) 
	: mVertexPath(vertexPath), mFragmentPath(fragmentPath), mGeometryPath(geometryPath)
{
	CreateAndLinkProgram();
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

void Shader::CreateAndLinkProgram() {
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	try
	{
		vertexCode = RetrieveShaderCode(mVertexPath.c_str());
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::VERTEX_SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		std::cout << "Shader file path: " << mVertexPath << std::endl;
	}

	try
	{
		fragmentCode = RetrieveShaderCode(mFragmentPath.c_str());
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::FRAGMENT_SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		std::cout << "Shader file path: " << mFragmentPath << std::endl;
	}

	try
	{
		if (!mGeometryPath.empty())
		{
			geometryCode = RetrieveShaderCode(mGeometryPath.c_str());
		}
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::GEOMETRY_SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		std::cout << "Shader file path: " << mGeometryPath << std::endl;
	}

	unsigned int vertex = CreateAndCompileShader(vertexCode, GL_VERTEX_SHADER);
	unsigned int fragment = CreateAndCompileShader(fragmentCode, GL_FRAGMENT_SHADER);
	unsigned int geometry = 0;
	if (!mGeometryPath.empty()) {
		geometry = CreateAndCompileShader(fragmentCode, GL_GEOMETRY_SHADER);
	}

	mProgramID = glCreateProgram();
	glAttachShader(mProgramID, vertex);
	glAttachShader(mProgramID, fragment);
	if (geometry != 0)
		glAttachShader(mProgramID, geometry);

	glLinkProgram(mProgramID);
	CheckLinkErrors(mProgramID);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (!mGeometryPath.empty())
		glDeleteShader(geometry);
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

unsigned int Shader::CreateAndCompileShader(std::string shaderCode, GLenum shaderType) {
	const char* gShaderCode = shaderCode.c_str();
	unsigned int shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &gShaderCode, NULL);
	glCompileShader(shaderID);
	CheckCompileErrors(shaderID, shaderType);

	return shaderID;
}

std::string Shader::RetrieveShaderCode(const char* shaderFileSrc) {
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	std::stringstream shaderStream;
	shaderFile.open(shaderFileSrc);
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();

	return shaderStream.str();
}

void Shader::CheckCompileErrors(GLuint shader, GLenum shaderType)
{
	GLint success;
	GLchar infoLog[1024];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER_COMPILATION_ERROR" << "\n"
			<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}
}

void Shader::CheckLinkErrors(GLuint program)
{
	GLint success;
	GLchar infoLog[1024];

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 1024, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR" << "\n"
			<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}
}