#include "ShaderLoader.hpp"
#include "src/utils/fileUtils.h"

static const std::string SHADER_LIB_PATH = fileUtils::getFullResourcesPath("shaders/lib/");

GLuint ShaderLoader::Load(
	const std::string& vertexPath,
	const std::string& fragmentPath,
	const std::string& geometryPath) 
{
	std::string vertexCode;
	try
	{
		vertexCode = RetrieveCode(vertexPath);
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::VERTEX_SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		std::cout << "Shader file path: " << vertexPath << std::endl;
	}

	std::string fragmentCode;
	try
	{
		fragmentCode = RetrieveCode(fragmentPath);
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::FRAGMENT_SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		std::cout << "Shader file path: " << fragmentPath << std::endl;
	}

	std::string geometryCode;
	try
	{
		if (!geometryPath.empty())
		{
			geometryCode = RetrieveCode(geometryPath);
		}
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::GEOMETRY_SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		std::cout << "Shader file path: " << geometryPath << std::endl;
	}

	unsigned int vertex = CreateAndCompileShader(vertexCode, GL_VERTEX_SHADER);
	unsigned int fragment = CreateAndCompileShader(fragmentCode, GL_FRAGMENT_SHADER);
	unsigned int geometry = 0;
	if (!geometryPath.empty()) {
		geometry = CreateAndCompileShader(fragmentCode, GL_GEOMETRY_SHADER);
	}

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertex);
	glAttachShader(programID, fragment);
	if (geometry != 0)
		glAttachShader(programID, geometry);

	glLinkProgram(programID);
	CheckLinkErrors(programID);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (!geometryPath.empty())
		glDeleteShader(geometry);

	return programID;
}

std::string ShaderLoader::RetrieveCode(const std::string& path, std::string includeIndentifier)
{
	includeIndentifier += ' ';

	std::string sourceCode = "";
	std::ifstream file(path);

	if (!file.is_open())
	{
		std::cerr << "ERROR: could not open the shader at: " << path << "\n" << std::endl;
		return sourceCode;
	}

	std::string lineBuffer;
	while (std::getline(file, lineBuffer))
	{
		bool isInclude = lineBuffer.compare(0, includeIndentifier.length(), includeIndentifier) == 0;
		if (isInclude)
		{
			auto includePath = GetIncludePath(lineBuffer, includeIndentifier, path);
			sourceCode += RetrieveCode(includePath);
		}
		else {
			sourceCode += lineBuffer + '\n';
		}
	}

	file.close();

	return sourceCode;
}

std::string ShaderLoader::GetIncludePath(const std::string& lineBuffer, const std::string& includeIndentifier, const std::string& shaderPath) {
	std::string includeLine = lineBuffer;
	includeLine.erase(0, includeIndentifier.size());

	if (includeLine.front() == '<' && includeLine.back() == '>') {
		includeLine = SHADER_LIB_PATH + includeLine.substr(1, includeLine.size() - 2); // -2 because last character is '\0'
	}
	else {
		std::string folderPath = shaderPath;
		std::string slash = "/\\";
		size_t posSlash = folderPath.find_last_of(slash);
		folderPath.erase(posSlash, std::string::npos);

		std::string upOneLevel = "../";
		size_t posUpOneLevel = includeLine.find(upOneLevel);
		while (posUpOneLevel != std::string::npos) {
			includeLine.erase(0, upOneLevel.length());
			posUpOneLevel = includeLine.find(upOneLevel);

			posSlash = folderPath.find_last_of(slash);
			folderPath.erase(posSlash, std::string::npos);
		}

		includeLine = folderPath + "/" + includeLine;
	}

	return includeLine;
}

unsigned int ShaderLoader::CreateAndCompileShader(std::string shaderCode, GLenum shaderType) {
	const char* gShaderCode = shaderCode.c_str();
	unsigned int shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &gShaderCode, NULL);
	glCompileShader(shaderID);
	CheckCompileErrors(shaderID, shaderType);

	return shaderID;
}

void ShaderLoader::CheckCompileErrors(GLuint shader, GLenum shaderType)
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

void ShaderLoader::CheckLinkErrors(GLuint program)
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