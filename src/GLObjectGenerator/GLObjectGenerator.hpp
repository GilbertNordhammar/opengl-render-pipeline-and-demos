#pragma once
#include "src/GLObjectArray/GLObjectArray.hpp"
class GLObjectGenerator
{
public:
	static GLObjectArray GenBuffers(unsigned int count);
	static GLObjectArray GenVertexArrays(unsigned int count);
	static GLObjectArray GenTexture(unsigned int count);
};

