#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <iostream>

class ShaderLoader
{
public:
	// Return the source code of the complete shader
	static GLuint Load(
		const std::string& vertexPath,
		const std::string& fragmentPath,
		const std::string& geometryPath);
	static std::string RetrieveCode(const std::string& path, std::string includeIndentifier = "#include");
private:
	static std::string GetIncludePath(const std::string& lineBuffer, const std::string& includeIndentifier, const std::string& shaderPath);
	static void CreateAndLinkProgram();
	static unsigned int CreateAndCompileShader(std::string shaderCode, GLenum shaderType);
	static void CheckCompileErrors(GLuint shader, GLenum shaderType, const std::string& shaderPath);
	static void CheckLinkErrors(GLuint program);
};