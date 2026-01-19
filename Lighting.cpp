#include "Lighting.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <string>

namespace Example
{
    LightingManager::LightingManager()
    {
                directionalLight = DirectionalLight(
            { -0.2f, -1.0f, -0.3f },              { 1.0f, 0.95f, 0.9f },                0.4f                             );
    }

    void LightingManager::addPointLight(const PointLight& light)
    {
        if (pointLights.size() < 16)
        {
            pointLights.push_back(light);
        }
    }

    void LightingManager::addSpotLight(const SpotLight& light)
    {
        if (spotLights.size() < 8)
        {
            spotLights.push_back(light);
        }
    }

    void LightingManager::togglePointLight(int index)
    {
        if (index >= 0 && index < (int)pointLights.size())
        {
            pointLights[index].enabled = !pointLights[index].enabled;
        }
    }

    void LightingManager::toggleSpotLight(int index)
    {
        if (index >= 0 && index < (int)spotLights.size())
        {
            spotLights[index].enabled = !spotLights[index].enabled;
        }
    }

    void LightingManager::toggleAllLights()
    {
        lightsOn = !lightsOn;  
        for (auto& light : pointLights)
            light.enabled = !light.enabled;
        for (auto& light : spotLights)
            light.enabled = !light.enabled;
    }

    void LightingManager::applyToShader(GLuint shaderProgram) const
    {
        glUseProgram(shaderProgram);

                                glUniform3fv(glGetUniformLocation(shaderProgram, "ambientColor"), 1,
            glm::value_ptr(ambientColor));
        glUniform1f(glGetUniformLocation(shaderProgram, "ambientIntensity"),
            ambientIntensity);

                                glUniform3fv(glGetUniformLocation(shaderProgram, "dirLight.direction"), 1,
            glm::value_ptr(directionalLight.direction));
        glUniform3fv(glGetUniformLocation(shaderProgram, "dirLight.color"), 1,
            glm::value_ptr(directionalLight.color));
        glUniform1f(glGetUniformLocation(shaderProgram, "dirLight.intensity"),
            directionalLight.intensity);

                                glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1,
            glm::value_ptr(viewPosition));

                                int numPointLights = std::min((int)pointLights.size(), 16);
        glUniform1i(glGetUniformLocation(shaderProgram, "numPointLights"), numPointLights);

        for (int i = 0; i < numPointLights; i++)
        {
            std::string base = "pointLights[" + std::to_string(i) + "]";

            glUniform3fv(glGetUniformLocation(shaderProgram, (base + ".position").c_str()), 1,
                glm::value_ptr(pointLights[i].position));
            glUniform3fv(glGetUniformLocation(shaderProgram, (base + ".color").c_str()), 1,
                glm::value_ptr(pointLights[i].color));
            glUniform1f(glGetUniformLocation(shaderProgram, (base + ".intensity").c_str()),
                pointLights[i].enabled ? pointLights[i].intensity : 0.0f);
            glUniform1f(glGetUniformLocation(shaderProgram, (base + ".radius").c_str()),
                pointLights[i].radius);
        }

                                int numSpotLights = std::min((int)spotLights.size(), 8);
        glUniform1i(glGetUniformLocation(shaderProgram, "numSpotLights"), numSpotLights);

        for (int i = 0; i < numSpotLights; i++)
        {
            std::string base = "spotLights[" + std::to_string(i) + "]";

            glUniform3fv(glGetUniformLocation(shaderProgram, (base + ".position").c_str()), 1,
                glm::value_ptr(spotLights[i].position));
            glUniform3fv(glGetUniformLocation(shaderProgram, (base + ".direction").c_str()), 1,
                glm::value_ptr(spotLights[i].direction));
            glUniform3fv(glGetUniformLocation(shaderProgram, (base + ".color").c_str()), 1,
                glm::value_ptr(spotLights[i].color));
            glUniform1f(glGetUniformLocation(shaderProgram, (base + ".intensity").c_str()),
                spotLights[i].enabled ? spotLights[i].intensity : 0.0f);
            glUniform1f(glGetUniformLocation(shaderProgram, (base + ".cutOff").c_str()),
                glm::cos(glm::radians(spotLights[i].cutoffAngle)));
            glUniform1f(glGetUniformLocation(shaderProgram, (base + ".outerCutOff").c_str()),
                glm::cos(glm::radians(spotLights[i].outerCutoffAngle)));
        }
    }
}