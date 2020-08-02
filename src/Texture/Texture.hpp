#pragma once
#include <string>
#include <assimp/scene.h>

class Texture
{
public:
    Texture(std::string path, aiTextureType type);

    unsigned int GetId() { return mId; }
    aiTextureType GetType() { return mType; }
    std::string GetFilePath() { return mFilePath; }

private:
    unsigned int mId;
    aiTextureType mType;
    std::string mFilePath;

    static unsigned int LoadTexture(const char* path);
};