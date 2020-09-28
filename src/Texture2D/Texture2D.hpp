#pragma once
#include <string>
#include <assimp/scene.h>
#include <glad/glad.h>
#include "src/GLObjectGenerator/GLObjectGenerator.hpp"
#include <memory>
#include <unordered_map>

class Texture2D
{
public:
    Texture2D(const Texture2D& other) = delete;
    Texture2D(Texture2D&& other) noexcept;

    Texture2D& operator=(Texture2D& other) = delete;
    Texture2D& operator=(Texture2D&& other) noexcept;

    GLuint GetId() const { return mTexture.GetFirst(); }
    aiTextureType GetType() const { return mType; }
    const std::string& GetFilePath() const { return mFilePath; }

    static std::shared_ptr<Texture2D> Generate(const std::string& path, aiTextureType type);
private:
    GLObjectArray mTexture;
    aiTextureType mType;
    std::string mFilePath; 

    Texture2D(std::string path, aiTextureType type);
    void LoadTexture(const char* path);
    
    static std::unordered_map<std::string, std::shared_ptr<Texture2D>> mLoadedTextures;
};