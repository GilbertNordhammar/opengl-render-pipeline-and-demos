#pragma once
#include <glad/glad.h>
#include <vector>

class GLObjectArrayInterface {
public:
	virtual void Push(unsigned int nObjects) = 0;
	virtual void Remove(GLuint object) = 0;
	virtual const std::vector<GLuint>& GetObjects() const = 0;
	virtual GLuint GetFirst() const = 0;
};