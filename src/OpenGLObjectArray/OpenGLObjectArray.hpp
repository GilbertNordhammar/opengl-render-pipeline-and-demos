#pragma once
#include <glad/glad.h>
#include <vector>
#include <functional>

class OpenGLObjectArray {
public:
	OpenGLObjectArray(const std::function<void(unsigned int, GLuint*)> funcGenerateObjects,
				  const std::function<void(unsigned int, GLuint*)> funcDeleteObjects);
	
	OpenGLObjectArray(unsigned int nObjects,
				  const std::function<void(unsigned int, GLuint*)> funcGenerateObjects,
			      const std::function<void(unsigned int, GLuint*)> funcDeleteObjects);
	OpenGLObjectArray(const OpenGLObjectArray& other) = delete;
	OpenGLObjectArray(OpenGLObjectArray&& other);
	~OpenGLObjectArray();

	OpenGLObjectArray& operator=(OpenGLObjectArray& other) = delete;
	OpenGLObjectArray& operator=(OpenGLObjectArray&& other) noexcept;

	virtual void Push(unsigned int nObjects);
	virtual void Remove(GLuint object);

	virtual const std::vector<GLuint>& GetObjects() const { return mObjects; }
	GLuint GetFirst() const { return mObjects[0]; }
private:
	std::vector<GLuint> mObjects;

	std::function<void(unsigned int, GLuint*)> GenerateObjects;
	std::function<void(unsigned int, GLuint*)> DeleteObjects;
	
	void CleanUp();
	void Move(OpenGLObjectArray& other);
};