#pragma once
#include "../GLObjectArrayInterface.hpp"
#include <vector>

class VAOArray : public GLObjectArrayInterface
{
public:
	VAOArray() {};
	VAOArray(unsigned int nObjects);
	VAOArray(const VAOArray& other) = delete;
	VAOArray(VAOArray&& other);
	~VAOArray();

	VAOArray& operator=(VAOArray& other) = delete;
	VAOArray& operator=(VAOArray&& other) noexcept;

	virtual void Push(unsigned int nObjects) override;
	virtual void Remove(GLuint object) override;

	virtual const std::vector<GLuint>& GetObjects() const override { return mObjects; }
	GLuint GetFirst() const { return mObjects[0]; }
private:
	std::vector<GLuint> mObjects;
	
	void CleanUp();
	void Move(VAOArray& other);
};



