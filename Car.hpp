#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "basic-shape.hpp"

namespace Example
{
    class Car
    {
    private:
        // ═══════════════════════════════════════════════════════════
        // الهيكل الخارجي
        // ═══════════════════════════════════════════════════════════
        BasicShape bodyMain;          // الجسم الرئيسي
        BasicShape bodyHood;          // غطاء المحرك
        BasicShape bodyTrunk;         // صندوق السيارة الخلفي
        BasicShape bodyRoof;          // السقف

        // ═══════════════════════════════════════════════════════════
        // النوافذ والزجاج
        // ═══════════════════════════════════════════════════════════
        BasicShape windowFront;       // الزجاج الأمامي
        BasicShape windowRear;        // الزجاج الخلفي
        BasicShape windowsLeft;       // النوافذ اليسارية
        BasicShape windowsRight;      // النوافذ اليمينية

        // ═══════════════════════════════════════════════════════════
        // العجلات
        // ═══════════════════════════════════════════════════════════
        BasicShape wheels;            // الإطارات الأربعة
        BasicShape wheelRims;         // جنوط العجلات

        // ═══════════════════════════════════════════════════════════
        // المصابيح
        // ═══════════════════════════════════════════════════════════
        BasicShape headlights;        // المصابيح الأمامية
        BasicShape taillights;        // المصابيح الخلفية
        BasicShape indicators;        // الإشارات

        // ═══════════════════════════════════════════════════════════
        // الداخلية
        // ═══════════════════════════════════════════════════════════
        BasicShape seats;             // المقاعد
        BasicShape steeringWheel;     // المقود
        BasicShape dashboard;         // لوحة القيادة
        BasicShape interiorPanels;    // الألواح الداخلية
        BasicShape floorMat;          // أرضية السيارة
        BasicShape gearShift;         // ناقل الحركة
        BasicShape mirrors;           // المرايا

        // ═══════════════════════════════════════════════════════════
        // التفاصيل
        // ═══════════════════════════════════════════════════════════
        BasicShape doorHandles;       // مقابض الأبواب
        BasicShape grille;            // شبك أمامي
        BasicShape bumpers;           // المصدات

        float baseY = 0.0f;
        float steer = 0.0f;            // إدخال الستيرينغ
        float turnRate = 80.0f;        // سرعة الدوران (درجات/ثانية)


        // -------- Driver Door Animation --------
        BasicShape driverDoor;                 // شكل باب السائق (لوح بسيط)
        glm::vec3 driverDoorHingeLocal = { 0,0,0 }; // موقع المفصلة داخل السيارة (محلي)
        float driverDoorAngle = 0.0f;          // زاوية حالية
        float driverDoorTargetAngle = 0.0f;    // زاوية الهدف
        float driverDoorOpenAngle = -70.0f;    // افتح للخارج (لجهة Z السالب)

        // -------- Car Movement --------
        float throttle = 0.0f;        // -1 للخلف, +1 للأمام
        float speed = 0.0f;           // سرعة حالية
        float maxForwardSpeed = 10.0f;
        float maxReverseSpeed = 4.0f;
        float acceleration = 12.0f;   // تسارع
        float friction = 10.0f;       // احتكاك لإيقاف السيارة بسرعة

        // موقع السيارة
        glm::vec3 position;
        float rotation;               // زاوية الدوران حول Y

        // أبعاد السيارة
        float length = 4.5f;          // الطول
        float width = 1.9f;           // العرض
        float height = 1.4f;          // الارتفاع
        float groundClearance = 0.2f; // الارتفاع عن الأرض

        // ألوان السيارة
        glm::vec3 bodyColor;
        glm::vec3 interiorColor;

        // حالة الباب
        bool driverDoorOpen = false;

        // دوال مساعدة للبناء
        void buildExterior();
        void buildInterior();
        void buildWheels();
        void buildLights();
        void buildDetails();

        void buildDriverDoor();

    public:
        Car();
        ~Car() = default;

        

        

        // تهيئة السيارة
        void create(glm::vec3 pos, float rotationY = 0.0f,
            glm::vec3 bodyCol = { 0.7f, 0.1f, 0.1f },      // أحمر افتراضي
            glm::vec3 interiorCol = { 0.15f, 0.12f, 0.1f }); // بني غامق

        // رسم السيارة
        void render(const glm::mat4& viewProj);

        // الحصول على موقع مقعد السائق (للكاميرا)
        glm::vec3 getDriverSeatPosition() const;

        // الحصول على اتجاه النظر من مقعد السائق
        glm::vec3 getDriverViewDirection() const;

        // فحص إذا كان اللاعب قريب من باب السائق
        bool isNearDriverDoor(glm::vec3 playerPos) const;

        // الحصول على حدود التصادم
        void getCollisionBounds(float& minX, float& maxX, float& minZ, float& maxZ) const;

        // Getters
        void setSteer(float s);  // -1 يسار, +1 يمين
        void setPosition(const glm::vec3& p) { position = p; }
        void stopMovement() { speed = 0.0f; throttle = 0.0f; steer = 0.0f; }
        float getGroundClearance() const { return groundClearance; }

        glm::vec3 getPosition() const { return position; }
        float getRotation() const { return rotation; }


        void update(float dt);                 // تحديث الباب + الحركة
        void setDriverDoorOpen(bool open);     // فتح/إغلاق باب السائق
        void setThrottle(float t);             // t من -1 إلى +1
        float getSpeed() const { return speed; }
    };
}