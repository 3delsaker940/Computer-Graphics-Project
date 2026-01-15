#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Example
{
	struct BasicVertex
	{
		glm::vec3 position;
		glm::vec3 color;
	};

	class BasicShape
	{
	protected:
		int verticesCount = 0;

		GLuint cameraLocation = 0, transformLocation = 0;
		GLuint alphaLocation = 0;  // ✅ جديد: موقع الشفافية
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

		// ✅ تعديل: إضافة parameter للشفافية
		void render(const glm::mat4& transform = glm::mat4(1.0f),
			const glm::mat4& camera = glm::mat4(1.0f),
			float alpha = 1.0f) const;
	};
}