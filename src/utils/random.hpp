#pragma once
#include <stdlib.h>
#include <time.h>
#include <random>
#include <iostream>

static std::random_device rd;
static std::mt19937 gen(rd());

namespace randomUtils {
	template<typename T>
	std::vector<T> uniformRandomDecimal(T min, T max, unsigned int numbElements) {
		static_assert(std::is_floating_point<T>::value, "T must be a float, double or long double");
		std::vector<T> randomValues;
		randomValues.reserve(numbElements);

		std::uniform_real_distribution<T> dis(min, max);
		for (int i = 0; i < numbElements; i++) {
			randomValues.push_back(dis(gen));
		}

		return randomValues;
	}

	std::vector<int> uniformRandomInt(int min, int max, unsigned int numbElements) {
		std::vector<int> randomValues;
		randomValues.reserve(numbElements);

		std::uniform_int_distribution dis(min, max);
		for (int i = 0; i < numbElements; i++) {
			randomValues.push_back(dis(gen));
		}

		return randomValues;
	}
}