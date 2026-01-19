#include <glad/glad.h>

#include <iostream>
#include <string>
#include <vector>

#include "shader-utils.hpp"
#include "basic-shape.hpp"


static const char* BASIC_VERTEX_SHADER_SRC = R"(
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec3 Color;

uniform mat4 camera;
uniform mat4 transform;

void main()
{
        vec4 worldPos = transform * vec4(aPos, 1.0);
    FragPos = vec3(worldPos);
    
        mat3 normalMatrix = transpose(inverse(mat3(transform)));
    Normal = normalize(normalMatrix * aNormal);
    
    Color = aColor;
    
    gl_Position = camera * worldPos;
}
)";

static const char* BASIC_FRAGMENT_SHADER_SRC = R"(
#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

out vec4 FragColor;

uniform float alpha;
uniform vec3 viewPos;

uniform vec3 ambientColor;
uniform float ambientIntensity;

struct DirLight {
    vec3 direction;
    vec3 color;
    float intensity;
};
uniform DirLight dirLight;

#define MAX_POINT_LIGHTS 16
struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float radius;
};
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numPointLights;

#define MAX_SPOT_LIGHTS 8
struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float cutOff;
    float outerCutOff;
};
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int numSpotLights;


vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
        float diff = max(dot(normal, lightDir), 0.0);
    
        vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    vec3 diffuse = light.color * diff * light.intensity;
    vec3 specular = light.color * spec * light.intensity * 0.5;
    
    return diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
        float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + (distance / light.radius) * (distance / light.radius));
    
        float diff = max(dot(normal, lightDir), 0.0);
    
        vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    vec3 diffuse = light.color * diff * light.intensity * attenuation;
    vec3 specular = light.color * spec * light.intensity * attenuation * 0.5;
    
    return diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
        float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float spotIntensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    if (theta < light.outerCutOff)
        return vec3(0.0);
    
        float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
    
        float diff = max(dot(normal, lightDir), 0.0);
    
        vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    vec3 diffuse = light.color * diff * light.intensity * attenuation * spotIntensity;
    vec3 specular = light.color * spec * light.intensity * attenuation * spotIntensity * 0.5;
    
    return diffuse + specular;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
                
        vec3 ambient = ambientColor * ambientIntensity;
    
        vec3 result = calcDirLight(dirLight, norm, viewDir);
    
        for (int i = 0; i < numPointLights && i < MAX_POINT_LIGHTS; i++)
    {
        result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    
        for (int i = 0; i < numSpotLights && i < MAX_SPOT_LIGHTS; i++)
    {
        result += calcSpotLight(spotLights[i], norm, FragPos, viewDir);
    }
    
        vec3 finalColor = (ambient + result) * Color;
    
        finalColor = clamp(finalColor, 0.0, 1.5);
    
    FragColor = vec4(finalColor, alpha);
}
)";

namespace Example
{
    GLuint BasicShape::shaderProgram = 0;

    void BasicShape::compileShapeShader()
    {
        if (shaderProgram != 0)
            return;
        shaderProgram = compileAndLinkShaderProgram(BASIC_VERTEX_SHADER_SRC, BASIC_FRAGMENT_SHADER_SRC, "lit-shader");

        std::cout << " Lighting shader compiled successfully!" << std::endl;
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

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(BasicVertex), vertices.data(), GL_STATIC_DRAW);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex),
            (void*)offsetof(BasicVertex, position));
        glEnableVertexAttribArray(0);

                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex),
            (void*)offsetof(BasicVertex, color));
        glEnableVertexAttribArray(1);

                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex),
            (void*)offsetof(BasicVertex, normal));
        glEnableVertexAttribArray(2);

        cameraLocation = glGetUniformLocation(shaderProgram, "camera");
        transformLocation = glGetUniformLocation(shaderProgram, "transform");
        alphaLocation = glGetUniformLocation(shaderProgram, "alpha");

        glBindVertexArray(0);
    }

    BasicShape::BasicShape(BasicShape&& other) noexcept
        : verticesCount(other.verticesCount),
        cameraLocation(other.cameraLocation),
        transformLocation(other.transformLocation),
        alphaLocation(other.alphaLocation),
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
            alphaLocation = other.alphaLocation;
            VAO = other.VAO;
            VBO = other.VBO;
            drawMode = other.drawMode;

            other.verticesCount = 0;
            other.VAO = 0;
            other.VBO = 0;
        }

        return *this;
    }

    void BasicShape::render(const glm::mat4& transform, const glm::mat4& camera, float alpha) const
    {
        if (verticesCount == 0)
            return;

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glUniformMatrix4fv(cameraLocation, 1, GL_FALSE, glm::value_ptr(camera));
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform1f(alphaLocation, alpha);
        glDrawArrays(drawMode, 0, verticesCount);
    }
}