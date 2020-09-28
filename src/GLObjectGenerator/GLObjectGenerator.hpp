#pragma once
#include "src/GLObjectArray/GLObjectArray.hpp"
class GLObjectGenerator
{
public:
	GLObjectArray GenBuffers(unsigned int count);
	GLObjectArray GenVertexArrays(unsigned int count);
	static GLObjectArray GenTexture(unsigned int count);
};

