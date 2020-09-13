#include "VAOArray.hpp"

VAOArray::VAOArray(unsigned int nObjects) {
	Push(nObjects);
}

VAOArray::VAOArray(VAOArray&& other) {
	Move(other);
}

VAOArray::~VAOArray() {
	CleanUp();
}

void VAOArray::Move(VAOArray& other) {
	CleanUp();
	mObjects = std::move(other.mObjects);
	other.mObjects.clear();
}

void VAOArray::CleanUp() {
	if (mObjects.size() > 0) {
		glDeleteVertexArrays(mObjects.size(), &mObjects[0]);
		mObjects.clear();
	}
}

VAOArray& VAOArray::operator=(VAOArray&& other) noexcept {
	Move(other);
	return *this;
}

void VAOArray::Push(unsigned int nObjects) {
	if (nObjects == 0)
		return;

	unsigned int oldSize = mObjects.size();
	unsigned int newSize = oldSize + nObjects;
	mObjects.resize(newSize);
	glGenVertexArrays(nObjects, &mObjects[oldSize]);
}

void VAOArray::Remove(GLuint object) {
	auto pos = std::find(mObjects.begin(), mObjects.end(), object);
	if (pos != mObjects.end()) {
		int index = std::distance(mObjects.begin(), pos);
		glDeleteVertexArrays(1, &mObjects[index]);
		mObjects.erase(pos);
	}
}