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

    std::vector<Mesh> mMeshes;
private:
    std::string mDirectory;

    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<std::shared_ptr<Texture2D>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type);
};