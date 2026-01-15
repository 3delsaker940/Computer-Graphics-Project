#include <glad/glad.h>

#include <iostream>
#include <string>
#include <vector>

#include "shader-utils.hpp"
#include "basic-shape.hpp"

// ✅ تعديل الشيدر لدعم الشفافية
static const char* BASIC_VERTEX_SHADER_SRC = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 camera;
uniform mat4 transform;

void main() {
    gl_Position = camera * transform * vec4(aPos, 1.0);
    vertexColor = aColor;
}
)";

static const char* BASIC_FRAMGENT_SHADER_SRC = R"(
#version 330 core

in vec3 vertexColor;

out vec4 FragColor;

uniform float alpha;  // ✅ جديد

void main() {
    FragColor = vec4(vertexColor, alpha);  // ✅ استخدام alpha
}
)";

namespace Example
{
	GLuint BasicShape::shaderProgram = 0;

	void BasicShape::compileShapeShader()
	{
		if (shaderProgram != 0)
			return;
		shaderProgram = compileAndLinkShaderProgram(BASIC_VERTEX_SHADER_SRC, BASIC_FRAMGENT_SHADER_SRC, "basic-shader");
	}

	BasicShape::BasicShape()
	{
		verticesCount = 0;
	}

	BasicShape::BasicShape(const std::vector<BasicVertex>& vertices, GLenum drawMode)
	{
		verticesCount = static_cast<int>(vertices.size());
		if (verticesCount == 0)
			return;

		this->drawMode = drawMode;

		compileShapeShader();

		int size = verticesCount * sizeof(BasicVertex);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, size, vertices.data(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), (void*)(offsetof(BasicVertex, position)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), (void*)(offsetof(BasicVertex, color)));
		glEnableVertexAttribArray(1);

		cameraLocation = glGetUniformLocation(shaderProgram, "camera");
		transformLocation = glGetUniformLocation(shaderProgram, "transform");
		alphaLocation = glGetUniformLocation(shaderProgram, "alpha");  // ✅ جديد
	}

	BasicShape::BasicShape(BasicShape&& other) noexcept
		: verticesCount(other.verticesCount),
		cameraLocation(other.cameraLocation),
		transformLocation(other.transformLocation),
		alphaLocation(other.alphaLocation),  // ✅ جديد
		VAO(other.VAO),
		VBO(other.VBO),
		drawMode(other.drawMode)
	{
		other.verticesCount = 0;
		other.VAO = 0;
		other.VBO = 0;
	}

	BasicShape::~BasicShape()
	{
		if (VAO != 0)
			glDeleteVertexArrays(1, &VAO);
		if (VBO != 0)
			glDeleteBuffers(1, &VBO);
	}

	BasicShape& BasicShape::operator=(BasicShape&& other) noexcept
	{
		if (this != &other)
		{
			if (VAO != 0)
				glDeleteVertexArrays(1, &VAO);
			if (VBO != 0)
				glDeleteBuffers(1, &VBO);

			verticesCount = other.verticesCount;
			cameraLocation = other.cameraLocation;
			transformLocation = other.transformLocation;
			alphaLocation = other.alphaLocation;  // ✅ جديد
			VAO = other.VAO;
			VBO = other.VBO;
			drawMode = other.drawMode;

			other.verticesCount = 0;
			other.VAO = 0;
			other.VBO = 0;
		}

		return *this;
	}

	// ✅ تعديل render لدعم الشفافية
	void BasicShape::render(const glm::mat4& transform, const glm::mat4& camera, float alpha) const
	{
		if (verticesCount == 0)
			return;

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glUniformMatrix4fv(cameraLocation, 1, false, glm::value_ptr(camera));
		glUniformMatrix4fv(transformLocation, 1, false, glm::value_ptr(transform));
		glUniform1f(alphaLocation, alpha);  // ✅ إرسال الشفافية
		glDrawArrays(drawMode, 0, verticesCount);
	}
}