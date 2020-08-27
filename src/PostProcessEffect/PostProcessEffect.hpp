#pragma once
#include "src/Shader/Shader.hpp"

class PostProcessEffect
{
public:
	PostProcessEffect(const std::string& fragmentPath);
	void UseShader(GLuint screenTexture);
	
private:
	Shader mShader;
};

