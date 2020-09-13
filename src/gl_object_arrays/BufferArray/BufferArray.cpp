#include "BufferArray.hpp"

BufferArray::BufferArray(unsigned int nObjects) {
	Push(nObjects);
}

BufferArray::BufferArray(BufferArray&& other) {
	Move(other);
}

BufferArray::~BufferArray() {
	CleanUp();
}

BufferArray& BufferArray::operator=(BufferArray&& other) noexcept {
	Move(other);
	return *this;
}

void BufferArray::CleanUp() {
	if (mObjects.size() > 0) {
		glDeleteBuffers(mObjects.size(), &mObjects[0]);
		mObjects.clear();
	}
}

void BufferArray::Move(BufferArray& other) {
	CleanUp();
	mObjects = std::move(other.mObjects);
	other.mObjects.clear();
}

void BufferArray::Push(unsigned int nObjects) {
	if (nObjects == 0)
		return;

	unsigned int oldSize = mObjects.size();
	unsigned int newSize = oldSize + nObjects;
	mObjects.resize(newSize);
	glGenBuffers(nObjects, &mObjects[oldSize]);
}

void BufferArray::Remove(GLuint object) {
	auto pos = std::find(mObjects.begin(), mObjects.end(), object);
	if (pos != mObjects.end()) {
		int index = std::distance(mObjects.begin(), pos);
		glDeleteBuffers(1, &mObjects[index]);
		mObjects.erase(pos);
	}
}