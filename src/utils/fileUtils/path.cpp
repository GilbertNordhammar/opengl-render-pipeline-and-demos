#pragma once
#include <string>

namespace fileUtils {
	std::string getCwd() {
		char full[_MAX_PATH];
		_fullpath(full, ".\\", _MAX_PATH);
		return full;
	}

	std::string getFullResourcesPath(std::string resourcesPath) {
		return getCwd() + "resources/" + resourcesPath;
	}
}