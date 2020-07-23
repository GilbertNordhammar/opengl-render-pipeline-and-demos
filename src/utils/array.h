#pragma once

namespace arrayUtils {

	template <typename T, size_t n>
	int length(T (&array)[n]) {
		int length = sizeof(array) > 0 ? sizeof(array) / sizeof(array[0]) : 0;
		return length;
	}
}

