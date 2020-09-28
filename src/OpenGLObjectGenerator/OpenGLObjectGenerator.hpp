#pragma once
#include "src/OpenGLObjectArray/OpenGLObjectArray.hpp"
class OpenGLObjectGenerator
{
public:
	static OpenGLObjectArray GenBuffers(unsigned int count);
	static OpenGLObjectArray GenVertexArrays(unsigned int count);
	static OpenGLObjectArray GenTexture(unsigned int count);
};

