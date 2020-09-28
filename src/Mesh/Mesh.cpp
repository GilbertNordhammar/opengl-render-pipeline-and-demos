#include "Mesh.hpp"
#include <glad/glad.h>

const int MAX_NUMB_TEXTURES = 15;

GLObjectGenerator Mesh::mObjGenerator = GLObjectGenerator();

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures)
    : mVboAndEbo(Mesh::mObjGenerator.GenBuffers(2)), mVao(Mesh::mObjGenerator.GenVertexArrays(1)),
    mVertices(vertices), mIndices(indices), mTextures(textures)
{
    SetupMesh();
}

Mesh::Mesh(const Mesh& other) : Mesh(other.mVertices, other.mIndices, other.mTextures) {}

Mesh::Mesh(Mesh&& other) noexcept :
    mVertices(std::move(other.mVertices)), 
    mIndices(std::move(other.mIndices)),
    mTextures(std::move(other.mTextures)),
    mVboAndEbo(std::move(other.mVboAndEbo)),
    mVao(std::move(other.mVao)) {}

Mesh& Mesh::operator=(Mesh other) {
    Swap(*this, other);
    return *this;
}

void Mesh::Swap(Mesh& first, Mesh& second) {
    using std::swap;

    swap(first.mVertices, second.mVertices);
    swap(first.mIndices, second.mIndices);
    swap(first.mTextures, second.mTextures);
    swap(first.mVboAndEbo, second.mVboAndEbo);
    swap(first.mVao, second.mVao);
}

void Mesh::SetupMesh()
{
    glBindVertexArray(mVao.GetFirst());
    glBindBuffer(GL_ARRAY_BUFFER, mVboAndEbo.GetObjects()[0]);

    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboAndEbo.GetObjects()[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader) const
{
    shader.use();

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for (int i = 0; i < MAX_NUMB_TEXTURES; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    for (unsigned int i = 0; i < mTextures.size(); i++)
    {
        std::string number;
        std::string name;

        switch (mTextures[i]->GetType()) {
        case aiTextureType::aiTextureType_DIFFUSE:
            name = "texture_diffuse";
            number = std::to_string(diffuseNr++);
            break;
        case aiTextureType::aiTextureType_SPECULAR:
            name = "texture_specular";
            number = std::to_string(specularNr++);
            break;
        case aiTextureType::aiTextureType_NORMALS:
            name = "texture_normal";
            number = std::to_string(normalNr++);
            break;
        case aiTextureType::aiTextureType_HEIGHT:
            name = "texture_height";
            number = std::to_string(heightNr++);
            break;
        }

        shader.setInt(("material." + name + number).c_str(), i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mTextures[i]->GetId());
    }

    glBindVertexArray(mVao.GetFirst());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}
