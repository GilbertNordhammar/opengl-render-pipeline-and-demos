#include "OpenGLObjectArray.hpp"
#include <iostream>

OpenGLObjectArray::OpenGLObjectArray(const std::function<void(unsigned int, GLuint*)> funcGenerateObjects,
							 const std::function<void(unsigned int, GLuint*)> funcDeleteObjects) 
	: GenerateObjects(funcGenerateObjects), DeleteObjects(funcDeleteObjects) {}

OpenGLObjectArray::OpenGLObjectArray(unsigned int nObjects,
							 const std::function<void(unsigned int, GLuint*)> funcGenerateObjects,
							 const std::function<void(unsigned int, GLuint*)> funcDeleteObjects)
	: OpenGLObjectArray(funcGenerateObjects, funcDeleteObjects) 
{
	Push(nObjects);
 }

OpenGLObjectArray::OpenGLObjectArray(OpenGLObjectArray&& other) {
	Move(other);
}

OpenGLObjectArray::~OpenGLObjectArray() {
	CleanUp();
}

void OpenGLObjectArray::Move(OpenGLObjectArray& other) {
	CleanUp();
	mObjects = std::move(other.mObjects);
	GenerateObjects = std::move(other.GenerateObjects);
	DeleteObjects = std::move(other.DeleteObjects);

	other.mObjects.clear();
}

void OpenGLObjectArray::CleanUp() {
	if (mObjects.size() > 0) {
		DeleteObjects(mObjects.size(), &mObjects[0]);
		mObjects.clear();
	}
}

OpenGLObjectArray& OpenGLObjectArray::operator=(OpenGLObjectArray&& other) noexcept {
	Move(other);
	return *this;
}

void OpenGLObjectArray::Push(unsigned int nObjects) {
	if (nObjects == 0)
		return;

	unsigned int oldSize = mObjects.size();
	unsigned int newSize = oldSize + nObjects;
	mObjects.resize(newSize);
	GenerateObjects(nObjects, &mObjects[oldSize]);
}

void OpenGLObjectArray::Remove(GLuint object) {
	auto pos = std::find(mObjects.begin(), mObjects.end(), object);
	if (pos != mObjects.end()) {
		int index = std::distance(mObjects.begin(), pos);
		DeleteObjects(1, &mObjects[index]);
		mObjects.erase(pos);
	}
}