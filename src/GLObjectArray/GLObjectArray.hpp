#pragma once
#include <glad/glad.h>
#include <vector>
#include <functional>

class GLObjectArray {
public:
	GLObjectArray(const std::function<void(unsigned int, GLuint*)> funcGenerateObjects,
				  const std::function<void(unsigned int, GLuint*)> funcDeleteObjects);
	
	GLObjectArray(unsigned int nObjects,
				  const std::function<void(unsigned int, GLuint*)> funcGenerateObjects,
			      const std::function<void(unsigned int, GLuint*)> funcDeleteObjects);
	GLObjectArray(const GLObjectArray& other) = delete;
	GLObjectArray(GLObjectArray&& other);
	~GLObjectArray();

	GLObjectArray& operator=(GLObjectArray& other) = delete;
	GLObjectArray& operator=(GLObjectArray&& other) noexcept;

	virtual void Push(unsigned int nObjects);
	virtual void Remove(GLuint object);

	virtual const std::vector<GLuint>& GetObjects() const { return mObjects; }
	GLuint GetFirst() const { return mObjects[0]; }
private:
	std::vector<GLuint> mObjects;

	std::function<void(unsigned int, GLuint*)> GenerateObjects;
	std::function<void(unsigned int, GLuint*)> DeleteObjects;
	
	void CleanUp();
	void Move(GLObjectArray& other);
};