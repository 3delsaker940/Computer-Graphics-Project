#include <glad/glad.h>
#include <vector>
#include <string>

// مكتبات Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// مكتباتك
#include "basic-shape.hpp"

namespace Example {

    void loadModelToShape(BasicShape& outShape, const std::string& path, glm::vec3 color) {
        Assimp::Importer importer;

        // القراءة والمعالجة (Triangulate مهم جداً)
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_FlipUVs |
            aiProcess_JoinIdenticalVertices);

        if (!scene || !scene->mRootNode) {
            return; // فشل التحميل
        }

        std::vector<BasicVertex> allVertices;

        for (unsigned int m = 0; m < scene->mNumMeshes; m++) {
            aiMesh* mesh = scene->mMeshes[m];

            for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
                aiFace face = mesh->mFaces[f];
                for (unsigned int i = 0; i < face.mNumIndices; i++) {
                    unsigned int vIdx = face.mIndices[i];

                    BasicVertex v;

                    // ✅ تصحيح 1: استخدام position بدلاً من pos
                    v.position = glm::vec3(mesh->mVertices[vIdx].x, mesh->mVertices[vIdx].y, mesh->mVertices[vIdx].z);

                    // الحصول على المادة (Material) من المش
                    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
                    aiColor3D diffuseColor(1.0f, 1.0f, 1.0f);
                    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);

                    // استخدام اللون من الخامة
                    v.color = glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);

                    // النورمال
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

        // ✅ تصحيح 2: استخدام الـ Constructor والـ Assignment بدلاً من دالة create غير الموجودة
        // هذا السطر ينشئ كائناً جديداً وينقل بياناته إلى outShape
        outShape = BasicShape(allVertices, GL_TRIANGLES);
    }
}