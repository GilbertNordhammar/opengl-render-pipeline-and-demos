#pragma once
#include <string>
#include <assimp/scene.h>
#include <glad/glad.h>

class Texture2D
{
public:
    Texture2D(std::string path, aiTextureType type);

    GLuint GetId() const { return mId; }
    aiTextureType GetType() const { return mType; }
    std::string GetFilePath() const { return mFilePath; }

private:
    GLuint mId = 0;
    aiTextureType mType;
    std::string mFilePath;

    static unsigned int LoadTexture(const char* path);
};