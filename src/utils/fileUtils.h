#pragma once
#include <string>

namespace fileUtils {
	std::string getCwd();
	std::string getFullResourcesPath(std::string resourcesPath);
}