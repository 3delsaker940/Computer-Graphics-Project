#include <glad/glad.h>
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "basic-shape.hpp"

namespace Example {

    void loadModelToShape(BasicShape& outShape, const std::string& path, glm::vec3 color) {
        Assimp::Importer importer;

                const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_FlipUVs |
            aiProcess_JoinIdenticalVertices);

        if (!scene || !scene->mRootNode) {
            return;         }

        std::vector<BasicVertex> allVertices;

        for (unsigned int m = 0; m < scene->mNumMeshes; m++) {
            aiMesh* mesh = scene->mMeshes[m];

            for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
                aiFace face = mesh->mFaces[f];
                for (unsigned int i = 0; i < face.mNumIndices; i++) {
                    unsigned int vIdx = face.mIndices[i];

                    BasicVertex v;

                                        v.position = glm::vec3(mesh->mVertices[vIdx].x, mesh->mVertices[vIdx].y, mesh->mVertices[vIdx].z);

                                        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
                    aiColor3D diffuseColor(1.0f, 1.0f, 1.0f);
                    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);

                                        v.color = glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);

                                        if (mesh->HasNormals()) {
                        v.normal = glm::vec3(mesh->mNormals[vIdx].x, mesh->mNormals[vIdx].y, mesh->mNormals[vIdx].z);
                    }
                    else {
                        v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
                    }

                    allVertices.push_back(v);
                }
            }
        }

                        outShape = BasicShape(allVertices, GL_TRIANGLES);
    }
}