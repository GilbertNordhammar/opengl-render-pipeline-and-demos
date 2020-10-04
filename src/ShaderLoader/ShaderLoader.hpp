#pragma once

#include <string>
#include <fstream>
#include <iostream>

class ShaderLoader
{
public:
	// Return the source code of the complete shader
	static std::string Load(const std::string& path, std::string includeIndentifier = "#include");
private:
	static std::string GetIncludePath(const std::string& lineBuffer, const std::string& includeIndentifier, const std::string& shaderPath);
};