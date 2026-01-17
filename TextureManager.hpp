#pragma once

#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <iostream>

namespace Example
{
    class TextureManager
    {
    private:
        std::unordered_map<std::string, GLuint> textures;
        static TextureManager* instance;

    public:
        static TextureManager& getInstance()
        {
            static TextureManager instance;
            return instance;
        }

        // تحميل texture من ملف
        GLuint loadTexture(const std::string& name, const std::string& filepath,
            bool repeat = true, bool generateMipmaps = true);

        // إنشاء texture من ألوان (بدون ملف صورة)
        GLuint createColorTexture(const std::string& name, unsigned char r, unsigned char g,
            unsigned char b, unsigned char a = 255);

        // إنشاء texture شطرنجي
        GLuint createCheckerTexture(const std::string& name,
            unsigned char r1, unsigned char g1, unsigned char b1,
            unsigned char r2, unsigned char g2, unsigned char b2,
            int tileSize = 32);

        // إنشاء texture رخامي مُولَّد
        GLuint createMarbleTexture(const std::string& name,
            unsigned char baseR, unsigned char baseG, unsigned char baseB,
            float veinIntensity = 0.3f);

        // إنشاء texture معدني
        GLuint createMetallicTexture(const std::string& name,
            unsigned char r, unsigned char g, unsigned char b);

        // إنشاء texture خشبي
        GLuint createWoodTexture(const std::string& name,
            unsigned char baseR, unsigned char baseG, unsigned char baseB);

        // إنشاء texture concrete/إسمنت
        GLuint createConcreteTexture(const std::string& name,
            unsigned char r, unsigned char g, unsigned char b);

        // الحصول على texture
        GLuint getTexture(const std::string& name);

        // حذف texture
        void deleteTexture(const std::string& name);

        // حذف كل الـ textures
        void cleanup();

        // إنشاء كل الـ textures المطلوبة للمعرض
        void initShowroomTextures();

    private:
        TextureManager() = default;
        ~TextureManager() { cleanup(); }

        // دالة مساعدة لتوليد ضوضاء Perlin بسيطة
        float noise(float x, float y);
        float smoothNoise(float x, float y);
        float interpolatedNoise(float x, float y);
        float perlinNoise(float x, float y, int octaves = 4);
    };
}