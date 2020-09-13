#pragma once
#include <glm/glm.hpp>
#include "../Shader/Shader.hpp"
#include <vector>
#include <map>
#include <assimp/scene.h>
#include "src/Texture2D/Texture2D.hpp"
#include "src/GLObjectGenerator/GLObjectGenerator.hpp"

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
    Mesh(Mesh&& other) noexcept;

    Mesh& operator=(Mesh other);

    void Draw(Shader& shader) const;
private:
    GLObjectArray mVboAndEbo;
    GLObjectArray mVao;
    
    static GLObjectGenerator mObjGenerator;

    void Swap(Mesh& first, Mesh& second);
    void SetupMesh();
};