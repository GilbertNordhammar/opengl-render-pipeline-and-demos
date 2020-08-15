#pragma once
#include "src/Shader/Shader.hpp"

class PostProcessEffect
{
public:
	PostProcessEffect(const char* fragmentPath);
	void UseShader(GLuint screenTexture);
	
private:
	Shader mShader;
};

