#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>

namespace Example
{
    // ✅ تعديل: إضافة Normal للـ Vertex
    struct BasicVertex
    {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 normal;  // ✅ جديد

        BasicVertex() : position(0), color(0), normal(0, 1, 0) {}

        BasicVertex(glm::vec3 pos, glm::vec3 col, glm::vec3 norm = { 0, 1, 0 })
            : position(pos), color(col), normal(norm) {
        }
    };

    class BasicShape
    {
    protected:
        int verticesCount = 0;

        GLuint cameraLocation = 0, transformLocation = 0;
        GLuint alphaLocation = 0;
        GLuint VAO = 0, VBO = 0;

        GLenum drawMode = GL_TRIANGLES;

    public:
        static GLuint shaderProgram;
        static void compileShapeShader();

        BasicShape();
        BasicShape(const std::vector<BasicVertex>& vertices, GLenum drawMode = GL_TRIANGLES);
        ~BasicShape();

        BasicShape(BasicShape&& other) noexcept;
        BasicShape& operator=(BasicShape&& other) noexcept;

        BasicShape(const BasicShape&) = delete;
        BasicShape& operator=(const BasicShape&) = delete;

        void render(const glm::mat4& transform = glm::mat4(1.0f),
            const glm::mat4& camera = glm::mat4(1.0f),
            float alpha = 1.0f) const;

        // ✅ جديد: الحصول على الشيدر
        static GLuint getShaderProgram() { return shaderProgram; }
    };

    // ═══════════════════════════════════════════════════════════
    // دوال مساعدة لحساب Normals
    // ═══════════════════════════════════════════════════════════

    // حساب Normal لمثلث
    inline glm::vec3 calculateNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
    {
        glm::vec3 edge1 = p2 - p1;
        glm::vec3 edge2 = p3 - p1;
        return glm::normalize(glm::cross(edge1, edge2));
    }

    // إضافة مربع مع Normal صحيح
    inline void addQuadWithNormal(std::vector<BasicVertex>& verts,
        glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4,
        glm::vec3 color)
    {
        glm::vec3 normal = calculateNormal(p1, p2, p3);

        verts.push_back({ p1, color, normal });
        verts.push_back({ p2, color, normal });
        verts.push_back({ p3, color, normal });
        verts.push_back({ p1, color, normal });
        verts.push_back({ p3, color, normal });
        verts.push_back({ p4, color, normal });
    }

    void loadModelToShape(BasicShape& outShape, const std::string& path, glm::vec3 color);
}