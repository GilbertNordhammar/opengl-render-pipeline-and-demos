#include "ShaderLoader.hpp"
#include "src/utils/fileUtils.h"

/*
LICENCE
MIT License

Copyright (c) [2018] [Tahar Meijs]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

static const std::string SHADER_LIB_PATH = fileUtils::getFullResourcesPath("shaders/lib/");

std::string ShaderLoader::Load(const std::string& path, std::string includeIndentifier)
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
			sourceCode += Load(includePath);
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