#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "../Shader/Shader.hpp"
#include <vector>
#include <map>
#include <assimp/scene.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture {
    unsigned int id;
    aiTextureType type;
    std::string path;
};

class Mesh {
public:
    // mesh data
    std::vector<Vertex>       mVertices;
    std::vector<unsigned int> mIndices;
    std::vector<Texture>      mTextures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader& shader);
private:
    //  render data
    unsigned int mVAO, mVBO, mEBO;

    void SetupMesh();
};