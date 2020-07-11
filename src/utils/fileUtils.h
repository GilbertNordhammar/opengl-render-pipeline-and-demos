#pragma once
#include <string>

namespace fileUtils {
	std::string getCwd();
	std::string getFullPath(std::string srcPath);
}