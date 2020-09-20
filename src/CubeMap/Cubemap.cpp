#include "Cubemap.hpp"
#include <stb_image.h>
#include <iostream>

Cubemap::Cubemap(const std::vector<std::string>& pathsToFaces) :
    mPathsToFaces(pathsToFaces)
{
    mId = LoadCubemap(pathsToFaces);
}

Cubemap::Cubemap(std::string folder, std::vector<std::string> facesInFolder) {
    std::vector<std::string> pathsToFaces;
    pathsToFaces.reserve(facesInFolder.size());
    for (int i = 0; i < facesInFolder.size(); i++) {
        pathsToFaces.push_back(folder + "/" + facesInFolder[i]);
    }

    mId = LoadCubemap(pathsToFaces);
    mPathsToFaces = pathsToFaces;
}

Cubemap::Cubemap(const Cubemap& other) :
    Cubemap(other.mPathsToFaces) {}

Cubemap::Cubemap(Cubemap&& other) noexcept:
    mPathsToFaces(other.mPathsToFaces), mId(other.mId)
{
    other.mId = 0;
}

Cubemap& Cubemap::operator=(Cubemap other) {
    Swap(*this, other);

    return *this;
}

void Cubemap::Swap(Cubemap& first, Cubemap& second) {
    using std::swap;

    swap(first.mId, second.mId);
    swap(first.mPathsToFaces, second.mPathsToFaces);
}

Cubemap::~Cubemap() {
    glDeleteTextures(1, &mId); 
}

GLuint Cubemap::LoadCubemap(const std::vector<std::string>& pathsToFaces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    stbi_set_flip_vertically_on_load(false);
    for (unsigned int i = 0; i < pathsToFaces.size(); i++)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(pathsToFaces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << pathsToFaces[i] << std::endl;
        }
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}