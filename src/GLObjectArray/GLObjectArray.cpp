#include "GLObjectArray.hpp"
#include <iostream>

GLObjectArray::GLObjectArray(const std::function<void(unsigned int, GLuint*)> funcGenerateObjects,
							 const std::function<void(unsigned int, GLuint*)> funcDeleteObjects) 
	: GenerateObjects(funcGenerateObjects), DeleteObjects(funcDeleteObjects) {}

GLObjectArray::GLObjectArray(unsigned int nObjects,
							 const std::function<void(unsigned int, GLuint*)> funcGenerateObjects,
							 const std::function<void(unsigned int, GLuint*)> funcDeleteObjects)
	: GLObjectArray(funcGenerateObjects, funcDeleteObjects) 
{
	Push(nObjects);
 }

GLObjectArray::GLObjectArray(GLObjectArray&& other) {
	Move(other);
}

GLObjectArray::~GLObjectArray() {
	CleanUp();
}

void GLObjectArray::Move(GLObjectArray& other) {
	CleanUp();
	mObjects = std::move(other.mObjects);
	GenerateObjects = std::move(other.GenerateObjects);
	DeleteObjects = std::move(other.DeleteObjects);

	other.mObjects.clear();
}

void GLObjectArray::CleanUp() {
	if (mObjects.size() > 0) {
		DeleteObjects(mObjects.size(), &mObjects[0]);
		mObjects.clear();
	}
}

GLObjectArray& GLObjectArray::operator=(GLObjectArray&& other) noexcept {
	Move(other);
	return *this;
}

void GLObjectArray::Push(unsigned int nObjects) {
	if (nObjects == 0)
		return;

	unsigned int oldSize = mObjects.size();
	unsigned int newSize = oldSize + nObjects;
	mObjects.resize(newSize);
	GenerateObjects(nObjects, &mObjects[oldSize]);
}

void GLObjectArray::Remove(GLuint object) {
	auto pos = std::find(mObjects.begin(), mObjects.end(), object);
	if (pos != mObjects.end()) {
		int index = std::distance(mObjects.begin(), pos);
		DeleteObjects(1, &mObjects[index]);
		mObjects.erase(pos);
	}
}