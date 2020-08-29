#pragma once

#include "../Shader/Shader.hpp"
#include "../Mesh/Mesh.hpp"
#include <vector>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    Model(std::string path);
    Model(std::vector<Mesh> meshes);
    void Draw(Shader& shader);

private:
    // model data
    std::vector<Mesh> mMeshes;
    std::string mDirectory;
    std::vector<Texture2D> mLoadedTextures;

    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture2D> LoadMaterialTextures(aiMaterial* mat, aiTextureType type);
};