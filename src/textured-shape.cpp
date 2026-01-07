#include <glad/glad.h>

#include <iostream>
#include <string>

#include "shader-utils.hpp"
#include "textured-shape.hpp"

static const char* TEXTURED_VERTEX_SHADER_SRC = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aUV;

out vec3 vertexColor;
out vec2 textureCoord;

uniform mat4 camera;
uniform mat4 transform;

void main() {
gl_Position = camera * transform * vec4(aPos, 1.0);
vertexColor = aColor;
textureCoord = aUV;
}
)";

static const char* TEXTURED_FRAMGENT_SHADER_SRC = R"(
#version 330 core

in vec3 vertexColor;
in vec2 textureCoord;

out vec4 FragColor;

uniform sampler2D shapeTexture;

void main() {
FragColor = texture(shapeTexture, textureCoord) * vec4(vertexColor, 1.0);
}
)";

namespace Example
{
	GLuint TexturedShape::shaderProgram = 0;

	void TexturedShape::compileShapeShader()
	{
		if (shaderProgram != 0) return;
		shaderProgram = compileAndLinkShaderProgram(TEXTURED_VERTEX_SHADER_SRC, TEXTURED_FRAMGENT_SHADER_SRC, "textured-shader");
	}

	TexturedShape::TexturedShape()
	{
		verticesCount = 0;
	}

	TexturedShape::TexturedShape(const std::vector<TexturedVertex>& vertices, GLenum drawMode)
	{
		verticesCount = vertices.size();
		if (verticesCount == 0) return;

		this->drawMode = drawMode;

		compileShapeShader();

		int size = verticesCount * sizeof(TexturedVertex);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, size, vertices.data(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)(offsetof(TexturedVertex, position)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)(offsetof(TexturedVertex, color)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)(offsetof(TexturedVertex, uv)));
		glEnableVertexAttribArray(2);

		cameraLocation = glGetUniformLocation(shaderProgram, "camera");
		transformLocation = glGetUniformLocation(shaderProgram, "transform");
	}

	TexturedShape::~TexturedShape()
	{
		if (verticesCount == 0) return;

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteProgram(shaderProgram);

		verticesCount = 0;
	}

	TexturedShape& TexturedShape::operator=(TexturedShape&& other) noexcept
	{
		if (this != &other)
		{
			if (verticesCount != 0) this->~TexturedShape();

			verticesCount = other.verticesCount;
			shaderProgram = other.shaderProgram;
			cameraLocation = other.cameraLocation;
			transformLocation = other.transformLocation;
			VAO = other.VAO;
			VBO = other.VBO;

			drawMode = other.drawMode;

			other.verticesCount = 0;
		}

		return *this;
	}

	void TexturedShape::render(const glm::mat4& transform, const glm::mat4& camera) const
	{
		if (verticesCount == 0) return;

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glUniformMatrix4fv(cameraLocation, 1, false, glm::value_ptr(camera));
		glUniformMatrix4fv(transformLocation, 1, false, glm::value_ptr(transform));
		glDrawArrays(drawMode, 0, verticesCount);
	}
}
