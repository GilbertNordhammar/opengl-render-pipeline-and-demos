#include "Texture2D.hpp"
#include <stb_image.h>
#include <iostream>

// defining static variables
std::unordered_map<std::string, std::shared_ptr<Texture2D>> Texture2D::mLoadedTextures;

Texture2D::Texture2D(std::string path, aiTextureType type) 
    : mFilePath(path), mType(type), mTexture(OpenGLObjectGenerator::GenTexture(1)) 
{
    LoadTexture(path.c_str());
}

Texture2D::Texture2D(Texture2D&& other) noexcept
    : mFilePath(std::move(other.mFilePath)), mType(std::move(other.mType)),
      mTexture(std::move(other.mTexture)) {}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept {
    mFilePath = std::move(other.mFilePath);
    mType = std::move(other.mType);
    mTexture = std::move(other.mTexture);

    return *this;
}

void Texture2D::LoadTexture(const char* path)
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, mTexture.GetFirst());
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
    }
    stbi_image_free(data);
}

std::shared_ptr<Texture2D> Texture2D::Generate(const std::string& path, aiTextureType type) {
    auto cached = mLoadedTextures.find(path);
    if (cached != mLoadedTextures.end()) {
        return cached->second;
    }
    else {
        mLoadedTextures[path] = std::make_shared<Texture2D>(Texture2D(path, type));
        return mLoadedTextures[path];
    }
}