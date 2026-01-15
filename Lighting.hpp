#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

namespace Example
{
    // ═══════════════════════════════════════════════════════════
    // أنواع الإضاءة
    // ═══════════════════════════════════════════════════════════

    struct DirectionalLight
    {
        glm::vec3 direction;
        glm::vec3 color;
        float intensity;

        DirectionalLight(glm::vec3 dir = { -0.3f, -1.0f, -0.5f },
            glm::vec3 col = { 1.0f, 0.95f, 0.9f },
            float intens = 0.5f)
            : direction(glm::normalize(dir)), color(col), intensity(intens) {
        }
    };

    struct PointLight
    {
        glm::vec3 position;
        glm::vec3 color;
        float intensity;
        float radius;       // المدى
        bool enabled;

        PointLight(glm::vec3 pos = { 0, 0, 0 },
            glm::vec3 col = { 1.0f, 1.0f, 1.0f },
            float intens = 1.0f,
            float rad = 20.0f)
            : position(pos), color(col), intensity(intens), radius(rad), enabled(true) {
        }
    };

    struct SpotLight
    {
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 color;
        float intensity;
        float cutoffAngle;      // زاوية القطع الداخلية (بالدرجات)
        float outerCutoffAngle; // زاوية القطع الخارجية
        bool enabled;

        SpotLight(glm::vec3 pos = { 0, 5, 0 },
            glm::vec3 dir = { 0, -1, 0 },
            glm::vec3 col = { 1.0f, 1.0f, 1.0f },
            float intens = 2.0f,
            float cutoff = 25.0f,
            float outerCutoff = 35.0f)
            : position(pos), direction(glm::normalize(dir)), color(col),
            intensity(intens), cutoffAngle(cutoff), outerCutoffAngle(outerCutoff), enabled(true) {
        }
    };

    // ═══════════════════════════════════════════════════════════
    // مدير الإضاءة
    // ═══════════════════════════════════════════════════════════

    class LightingManager
    {
    public:
        // الإضاءة المحيطة
        glm::vec3 ambientColor = { 0.15f, 0.15f, 0.2f };
        float ambientIntensity = 0.3f;

        // الإضاءة الاتجاهية (الشمس/القمر)
        DirectionalLight directionalLight;

        // المصابيح النقطية (حتى 16 مصباح)
        std::vector<PointLight> pointLights;

        // المصابيح المركزة (حتى 8 مصابيح)
        std::vector<SpotLight> spotLights;

        // موقع الكاميرا (للانعكاس)
        glm::vec3 viewPosition;

    public:
        LightingManager();

        // إضافة مصابيح
        void addPointLight(const PointLight& light);
        void addSpotLight(const SpotLight& light);

        // تبديل حالة المصابيح
        void togglePointLight(int index);
        void toggleSpotLight(int index);
        void toggleAllLights();

        // إرسال بيانات الإضاءة للشيدر
        void applyToShader(GLuint shaderProgram) const;

        // تحديث موقع الكاميرا
        void setViewPosition(glm::vec3 pos) { viewPosition = pos; }
    };
}