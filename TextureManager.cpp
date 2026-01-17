#include "TextureManager.hpp"

// ═══════════════════════════════════════════════════════════════
// تضمين stb_image (مرة واحدة فقط في كل المشروع)
// ═══════════════════════════════════════════════════════════════
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>

namespace Example
{
    // ═══════════════════════════════════════════════════════════
    // تحميل texture من ملف صورة
    // ═══════════════════════════════════════════════════════════

    GLuint TextureManager::loadTexture(const std::string& name, const std::string& filepath,
        bool repeat, bool generateMipmaps)
    {
        // تحقق إذا كان موجوداً مسبقاً
        auto it = textures.find(name);
        if (it != textures.end())
            return it->second;

        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);

        unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

        if (!data)
        {
            std::cerr << "❌ Failed to load texture: " << filepath << std::endl;
            std::cerr << "   Reason: " << stbi_failure_reason() << std::endl;
            return 0;
        }

        GLenum format = GL_RGB;
        if (channels == 1) format = GL_RED;
        else if (channels == 3) format = GL_RGB;
        else if (channels == 4) format = GL_RGBA;

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        if (generateMipmaps)
            glGenerateMipmap(GL_TEXTURE_2D);

        // إعدادات الـ wrapping
        if (repeat)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        // إعدادات الفلترة
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            generateMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        textures[name] = textureID;
        std::cout << "✅ Texture loaded: " << name << " (" << width << "x" << height << ")" << std::endl;

        return textureID;
    }

    // ═══════════════════════════════════════════════════════════
    // إنشاء texture من لون واحد
    // ═══════════════════════════════════════════════════════════

    GLuint TextureManager::createColorTexture(const std::string& name,
        unsigned char r, unsigned char g,
        unsigned char b, unsigned char a)
    {
        auto it = textures.find(name);
        if (it != textures.end())
            return it->second;

        unsigned char data[4] = { r, g, b, a };

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        textures[name] = textureID;
        return textureID;
    }

    // ═══════════════════════════════════════════════════════════
    // إنشاء texture شطرنجي
    // ═══════════════════════════════════════════════════════════

    GLuint TextureManager::createCheckerTexture(const std::string& name,
        unsigned char r1, unsigned char g1, unsigned char b1,
        unsigned char r2, unsigned char g2, unsigned char b2,
        int tileSize)
    {
        auto it = textures.find(name);
        if (it != textures.end())
            return it->second;

        int size = tileSize * 8;  // 8x8 tiles
        std::vector<unsigned char> data(size * size * 3);

        for (int y = 0; y < size; y++)
        {
            for (int x = 0; x < size; x++)
            {
                int tileX = x / tileSize;
                int tileY = y / tileSize;
                bool isDark = (tileX + tileY) % 2 == 0;

                int idx = (y * size + x) * 3;

                if (isDark)
                {
                    data[idx + 0] = r1;
                    data[idx + 1] = g1;
                    data[idx + 2] = b1;
                }
                else
                {
                    data[idx + 0] = r2;
                    data[idx + 1] = g2;
                    data[idx + 2] = b2;
                }
            }
        }

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        textures[name] = textureID;
        std::cout << "✅ Checker texture created: " << name << std::endl;

        return textureID;
    }

    // ═══════════════════════════════════════════════════════════
    // دوال الضوضاء (Noise) للتوليد الإجرائي
    // ═══════════════════════════════════════════════════════════

    float TextureManager::noise(float x, float y)
    {
        int n = (int)x + (int)y * 57;
        n = (n << 13) ^ n;
        return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
    }

    float TextureManager::smoothNoise(float x, float y)
    {
        float corners = (noise(x - 1, y - 1) + noise(x + 1, y - 1) +
            noise(x - 1, y + 1) + noise(x + 1, y + 1)) / 16.0f;
        float sides = (noise(x - 1, y) + noise(x + 1, y) +
            noise(x, y - 1) + noise(x, y + 1)) / 8.0f;
        float center = noise(x, y) / 4.0f;
        return corners + sides + center;
    }

    float TextureManager::interpolatedNoise(float x, float y)
    {
        int intX = (int)x;
        float fracX = x - intX;
        int intY = (int)y;
        float fracY = y - intY;

        float v1 = smoothNoise((float)intX, (float)intY);
        float v2 = smoothNoise((float)intX + 1, (float)intY);
        float v3 = smoothNoise((float)intX, (float)intY + 1);
        float v4 = smoothNoise((float)intX + 1, (float)intY + 1);

        float i1 = v1 * (1 - fracX) + v2 * fracX;
        float i2 = v3 * (1 - fracX) + v4 * fracX;

        return i1 * (1 - fracY) + i2 * fracY;
    }

    float TextureManager::perlinNoise(float x, float y, int octaves)
    {
        float total = 0;
        float persistence = 0.5f;
        float frequency = 1;
        float amplitude = 1;
        float maxValue = 0;

        for (int i = 0; i < octaves; i++)
        {
            total += interpolatedNoise(x * frequency, y * frequency) * amplitude;
            maxValue += amplitude;
            amplitude *= persistence;
            frequency *= 2;
        }

        return total / maxValue;
    }

    // ═══════════════════════════════════════════════════════════
    // إنشاء texture رخامي
    // ═══════════════════════════════════════════════════════════

    GLuint TextureManager::createMarbleTexture(const std::string& name,
        unsigned char baseR, unsigned char baseG, unsigned char baseB,
        float veinIntensity)
    {
        auto it = textures.find(name);
        if (it != textures.end())
            return it->second;

        int size = 256;
        std::vector<unsigned char> data(size * size * 3);

        for (int y = 0; y < size; y++)
        {
            for (int x = 0; x < size; x++)
            {
                float nx = (float)x / size;
                float ny = (float)y / size;

                // نمط الرخام: خطوط متموجة
                float noise1 = perlinNoise(nx * 8, ny * 8, 4);
                float noise2 = perlinNoise(nx * 16, ny * 16, 4);

                float marble = std::sin((nx + noise1 * 0.5f) * 3.14159f * 4);
                marble = (marble + 1.0f) / 2.0f;  // normalize to 0-1

                // إضافة تفاصيل دقيقة
                float detail = noise2 * 0.15f;

                float intensity = marble * veinIntensity + (1.0f - veinIntensity) + detail;
                intensity = std::max(0.0f, std::min(1.0f, intensity));

                int idx = (y * size + x) * 3;
                data[idx + 0] = (unsigned char)(baseR * intensity);
                data[idx + 1] = (unsigned char)(baseG * intensity);
                data[idx + 2] = (unsigned char)(baseB * intensity);
            }
        }

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        textures[name] = textureID;
        std::cout << "✅ Marble texture created: " << name << std::endl;

        return textureID;
    }

    // ═══════════════════════════════════════════════════════════
    // إنشاء texture معدني
    // ═══════════════════════════════════════════════════════════

    GLuint TextureManager::createMetallicTexture(const std::string& name,
        unsigned char r, unsigned char g, unsigned char b)
    {
        auto it = textures.find(name);
        if (it != textures.end())
            return it->second;

        int size = 128;
        std::vector<unsigned char> data(size * size * 3);

        for (int y = 0; y < size; y++)
        {
            for (int x = 0; x < size; x++)
            {
                float nx = (float)x / size;
                float ny = (float)y / size;

                // نمط معدني: خطوط أفقية دقيقة + ضوضاء
                float brushed = std::sin(ny * 200) * 0.02f;
                float noise1 = perlinNoise(nx * 32, ny * 32, 3) * 0.08f;

                float intensity = 0.9f + brushed + noise1;
                intensity = std::max(0.7f, std::min(1.1f, intensity));

                int idx = (y * size + x) * 3;
                data[idx + 0] = (unsigned char)std::min(255.0f, r * intensity);
                data[idx + 1] = (unsigned char)std::min(255.0f, g * intensity);
                data[idx + 2] = (unsigned char)std::min(255.0f, b * intensity);
            }
        }

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        textures[name] = textureID;
        std::cout << "✅ Metallic texture created: " << name << std::endl;

        return textureID;
    }

    // ═══════════════════════════════════════════════════════════
    // إنشاء texture خشبي
    // ═══════════════════════════════════════════════════════════

    GLuint TextureManager::createWoodTexture(const std::string& name,
        unsigned char baseR, unsigned char baseG, unsigned char baseB)
    {
        auto it = textures.find(name);
        if (it != textures.end())
            return it->second;

        int size = 256;
        std::vector<unsigned char> data(size * size * 3);

        for (int y = 0; y < size; y++)
        {
            for (int x = 0; x < size; x++)
            {
                float nx = (float)x / size;
                float ny = (float)y / size;

                // حلقات الخشب
                float ring = std::sin((nx * 20 + perlinNoise(nx * 4, ny * 4, 3) * 2) * 3.14159f);
                ring = (ring + 1.0f) / 2.0f;

                // ألياف الخشب
                float fiber = perlinNoise(nx * 2, ny * 40, 4) * 0.1f;

                float intensity = ring * 0.3f + 0.7f + fiber;
                intensity = std::max(0.5f, std::min(1.0f, intensity));

                int idx = (y * size + x) * 3;
                data[idx + 0] = (unsigned char)(baseR * intensity);
                data[idx + 1] = (unsigned char)(baseG * intensity);
                data[idx + 2] = (unsigned char)(baseB * intensity);
            }
        }

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        textures[name] = textureID;
        std::cout << "✅ Wood texture created: " << name << std::endl;

        return textureID;
    }

    // ═══════════════════════════════════════════════════════════
    // إنشاء texture إسمنتي/Concrete
    // ═══════════════════════════════════════════════════════════

    GLuint TextureManager::createConcreteTexture(const std::string& name,
        unsigned char r, unsigned char g, unsigned char b)
    {
        auto it = textures.find(name);
        if (it != textures.end())
            return it->second;

        int size = 256;
        std::vector<unsigned char> data(size * size * 3);

        for (int y = 0; y < size; y++)
        {
            for (int x = 0; x < size; x++)
            {
                float nx = (float)x / size;
                float ny = (float)y / size;

                // ضوضاء متعددة المستويات للإسمنت
                float noise1 = perlinNoise(nx * 8, ny * 8, 4) * 0.15f;
                float noise2 = perlinNoise(nx * 32, ny * 32, 2) * 0.08f;
                float noise3 = perlinNoise(nx * 64, ny * 64, 2) * 0.04f;

                float intensity = 0.85f + noise1 + noise2 + noise3;
                intensity = std::max(0.6f, std::min(1.0f, intensity));

                int idx = (y * size + x) * 3;
                data[idx + 0] = (unsigned char)(r * intensity);
                data[idx + 1] = (unsigned char)(g * intensity);
                data[idx + 2] = (unsigned char)(b * intensity);
            }
        }

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        textures[name] = textureID;
        std::cout << "✅ Concrete texture created: " << name << std::endl;

        return textureID;
    }

    // ═══════════════════════════════════════════════════════════
    // دوال الإدارة
    // ═══════════════════════════════════════════════════════════

    GLuint TextureManager::getTexture(const std::string& name)
    {
        auto it = textures.find(name);
        if (it != textures.end())
            return it->second;
        return 0;
    }

    void TextureManager::deleteTexture(const std::string& name)
    {
        auto it = textures.find(name);
        if (it != textures.end())
        {
            glDeleteTextures(1, &it->second);
            textures.erase(it);
        }
    }

    void TextureManager::cleanup()
    {
        for (auto& pair : textures)
        {
            glDeleteTextures(1, &pair.second);
        }
        textures.clear();
    }

    // ═══════════════════════════════════════════════════════════
    // إنشاء كل الـ textures للمعرض
    // ═══════════════════════════════════════════════════════════

    void TextureManager::initShowroomTextures()
    {
        std::cout << "🎨 Initializing showroom textures..." << std::endl;

        // ─────────────────────────────────────────────────────
        // أرضيات الغرف
        // ─────────────────────────────────────────────────────

        // Luxury - رخام أسود
        createMarbleTexture("floor_luxury", 45, 45, 50, 0.25f);

        // Electric - بلاط أزرق داكن
        createCheckerTexture("floor_electric", 20, 30, 50, 30, 40, 65, 32);

        // Sports - بلاط رمادي محمر
        createCheckerTexture("floor_sports", 35, 28, 28, 50, 40, 40, 32);

        // Family - خشب دافئ
        createWoodTexture("floor_family", 120, 85, 60);

        // ─────────────────────────────────────────────────────
        // الجدران
        // ─────────────────────────────────────────────────────

        // جدران خرسانية حديثة
        createConcreteTexture("wall_concrete", 90, 90, 95);
        createConcreteTexture("wall_dark", 50, 50, 55);

        // ─────────────────────────────────────────────────────
        // المنصات
        // ─────────────────────────────────────────────────────

        // سطح معدني للمنصات
        createMetallicTexture("podium_top", 60, 60, 65);
        createMetallicTexture("podium_side", 40, 40, 45);

        // ─────────────────────────────────────────────────────
        // ديكورات
        // ─────────────────────────────────────────────────────

        // سجادة Luxury
        createColorTexture("carpet_luxury", 25, 20, 30);

        // لون ذهبي
        createColorTexture("gold", 220, 180, 60);

        // لون نيون
        createColorTexture("neon_blue", 50, 200, 255);
        createColorTexture("neon_red", 255, 50, 50);

        std::cout << "✅ All textures initialized!" << std::endl;
    }
}