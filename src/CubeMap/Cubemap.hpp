#pragma once
#include <glad/glad.h>
#include <string>
#include <vector>
#include <glad/glad.h>

class Cubemap
{
public:
	//Order of faces in vector: right, left, top, bottom, front, back
	Cubemap(const std::vector<std::string>& pathsToFaces);
	//Order of faces in vector: right, left, top, bottom, front, back
	Cubemap(std::string folder, std::vector<std::string> facesInFolder);
	Cubemap(const Cubemap& other);
	Cubemap(Cubemap&& other) noexcept;
	Cubemap& operator=(Cubemap other);

	~Cubemap();

	GLuint GetId() const { return mId; }
	const std::vector<std::string>& GetPathsToFaces() const { return mPathsToFaces; }
private:
	GLuint mId;
	std::vector<std::string> mPathsToFaces;

	static GLuint LoadCubemap(const std::vector<std::string>& pathsToFaces);
	static void Swap(Cubemap& first, Cubemap& second);
};

