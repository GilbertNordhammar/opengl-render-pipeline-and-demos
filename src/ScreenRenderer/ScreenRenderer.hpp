#pragma once
#include "src/PostProcessEffect/PostProcessEffect.hpp"
#include <string>
#include "src/utils/fileUtils.h"
#include <memory>

class ScreenRenderer
{
public:
	ScreenRenderer();
	void Draw(GLuint screenTexture, PostProcessEffect* shader);
	void Draw(GLuint screenTexture);

private:
	GLuint CreateScreenQuadVAO();

	PostProcessEffect mNoEffectPP;
	GLuint mScreenQuadVAO;
};

