#pragma once
#include "../GLObjectArrayInterface.hpp"
#include <vector>

class BufferArray : public GLObjectArrayInterface
{
public:
	BufferArray() {};
	BufferArray(unsigned int nObjects);
	BufferArray(const BufferArray& other) = delete;
	BufferArray(BufferArray&& other);
	~BufferArray();

	BufferArray& operator=(BufferArray& other) = delete;
	BufferArray& operator=(BufferArray&& other) noexcept;
	
	virtual void Push(unsigned int nObjects) override;
	virtual void Remove(GLuint object) override;

	virtual const std::vector<GLuint>& GetObjects() const override { return mObjects; }
	GLuint GetFirst() const { return mObjects[0]; }
private:
	std::vector<GLuint> mObjects;

	void CleanUp();
	void Move(BufferArray& other);
};

