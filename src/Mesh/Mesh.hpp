#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "../Shader/Shader.hpp"
#include <vector>
#include <map>
#include <assimp/scene.h>
#include "src/Texture2D/Texture2D.hpp"
#include "src/gl_object_arrays/BufferArray/BufferArray.hpp";
#include "src/gl_object_arrays/VAOArray/VAOArray.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

class Mesh {
public:
    std::vector<Vertex>       mVertices;
    std::vector<unsigned int> mIndices;
    std::vector<Texture2D>    mTextures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2D> textures);
    Mesh(const Mesh& other);
    ~Mesh() { std::cout << "destroyed Mesh " << mVboAndEbo.GetObjects()[0] << " " << mVboAndEbo.GetObjects()[1] << std::endl; }
    void Draw(Shader& shader);
private:
    BufferArray mVboAndEbo;
    VAOArray mVao;

    void SetupMesh();
};