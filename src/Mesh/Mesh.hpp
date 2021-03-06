#pragma once
#include <glm/glm.hpp>
#include "../Shader/Shader.hpp"
#include <vector>
#include <map>
#include <assimp/scene.h>
#include "src/Texture2D/Texture2D.hpp"
#include "src/OpenGLObjectGenerator/OpenGLObjectGenerator.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

class Mesh {
public:
    std::vector<Vertex>                     mVertices;
    std::vector<unsigned int>               mIndices;
    std::vector<std::shared_ptr<Texture2D>> mTextures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures);
    Mesh(const Mesh& other);
    Mesh(Mesh&& other) noexcept;

    Mesh& operator=(Mesh other);

    // Creates/recreates model matrix buffer and initializes it
    void SetInstancedModelMatrices(const std::vector<glm::mat4>& modelMatrices) const;

    // Updates existing model matrix buffer starting at byte bufferOffset * sizeof(glm::mat4)
    void UpdateInstancedModelMatrices(const std::vector<glm::mat4>& modelMatrices, int bufferOffset) const;
    
    void DrawInstanced(unsigned int count) const;
    void Draw() const;
    void PassTextures(const Shader& shader) const;
private:
    OpenGLObjectArray mVboAndEbo;
    OpenGLObjectArray mVao;
    OpenGLObjectArray mModelMatrices;

    void Swap(Mesh& first, Mesh& second);
    void SetupMesh();
};