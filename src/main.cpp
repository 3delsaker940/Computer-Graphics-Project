#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

#include "basic-shape.hpp"
#include "../Camera.hpp"
#include "../Showroom.hpp"
#include "../Car.hpp"

class Application
{
public:
    GLFWwindow* window;
    float lastFrameTime = 0.0f;

    Example::Camera appCamera;
    Example::Showroom showroom;

    // ✅ حالة الجلوس في السيارة
    bool isInCar = false;
    Example::Car* currentCar = nullptr;
    glm::vec3 savedPosition;
    float savedYaw, savedPitch;

    // ✅ للتحكم بالضغط على E
    bool eKeyWasPressed = false;

    // ✅ لعرض رسالة التفاعل
    bool nearCar = false;

    Application() : appCamera(glm::vec3(0.0f, 1.7f, 75.0f)), window(nullptr) {}

    void onInit()
    {
        Example::BasicShape::compileShapeShader();
        showroom.init();

        std::cout << "========================================" << std::endl;
        std::cout << "   Interactive Car Showroom" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "  W/A/S/D - Move" << std::endl;
        std::cout << "  Mouse   - Look around" << std::endl;
        std::cout << "  E       - Enter/Exit car" << std::endl;
        std::cout << "  ESC     - Quit" << std::endl;
        std::cout << "========================================" << std::endl;
    }

    void onUpdate()
    {
        float t = (float)glfwGetTime();
        float dt = t - lastFrameTime;
        lastFrameTime = t;

        // ═══════════════════════════════════════════════════════════
        // معالجة المدخلات
        // ═══════════════════════════════════════════════════════════

        if (!isInCar)
        {
            // ─────────────────────────────────────────
            // وضع المشي العادي
            // ─────────────────────────────────────────
            glm::vec3 oldPos = appCamera.Position;
            handleMovement(dt);
            appCamera.Position = showroom.checkCollision(oldPos, appCamera.Position);
            appCamera.Position.y = 1.7f;

            // فحص إذا كان قريب من سيارة
            Example::Car* nearbyCar = showroom.findNearestCar(appCamera.Position);
            nearCar = (nearbyCar != nullptr);

            // فحص الضغط على E للدخول
            bool ePressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
            if (ePressed && !eKeyWasPressed && nearbyCar != nullptr)
            {
                enterCar(nearbyCar);
            }
            eKeyWasPressed = ePressed;

            static bool fKeyWasPressed = false;
            bool fPressed = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
            if (fPressed && !fKeyWasPressed)
            {
                showroom.toggleNearestRoomDoor(appCamera.Position);
            }
            fKeyWasPressed = fPressed;
        }
        else
        {
            if (!currentCar)
            {
                isInCar = false;
                return;
            }

            // E للخروج (إذا خرجت لا تكمل نفس الفريم)
            bool ePressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
            if (ePressed && !eKeyWasPressed)
            {
                exitCar();
                eKeyWasPressed = ePressed;
                return; // ✅ مهم جداً لمنع استدعاء setThrottle بعد currentCar=nullptr
            }
            eKeyWasPressed = ePressed;

            // W/S للقيادة
            float tInput = 0.0f;
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) tInput += 1.0f;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) tInput -= 1.0f;

            currentCar->setThrottle(tInput);

            float sInput = 0.0f;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) sInput -= 1.0f;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) sInput += 1.0f;

            currentCar->setSteer(sInput);

            // بعد تحديث السيارة (showroom.update) سنثبت الكاميرا بالمقعد
        }


        // تحديث تحريك الأبواب
        showroom.update(dt);

        if (isInCar && currentCar)
        {
            appCamera.Position = currentCar->getDriverSeatPosition();
        }

        // ESC للخروج
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);


        // ✅ تحديث موقع الكاميرا للإضاءة
        showroom.lighting.setViewPosition(appCamera.Position);

        // ✅ زر L لتبديل الإضاءة
        static bool lKeyWasPressed = false;
        bool lPressed = glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS;
        if (lPressed && !lKeyWasPressed)
        {
            showroom.toggleLights();
        }
        lKeyWasPressed = lPressed;

        // ═══════════════════════════════════════════════════════════
        // الرسم
        // ═══════════════════════════════════════════════════════════

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        float aspectRatio = (float)width / (float)height;
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspectRatio, 0.1f, 300.0f);
        glm::mat4 viewProj = projection * appCamera.GetViewMatrix();

        // لون الخلفية
        if (isInCar)
        {
            glClearColor(0.02f, 0.02f, 0.03f, 1.0f);  // أغمق داخل السيارة
        }
        else
        {
            glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        showroom.renderAll(viewProj);

        // ═══════════════════════════════════════════════════════════
        // عرض حالة اللاعب في العنوان
        // ═══════════════════════════════════════════════════════════
        updateWindowTitle();
    }

private:
    void handleMovement(float dt)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            appCamera.ProcessKeyboard("FORWARD", dt);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            appCamera.ProcessKeyboard("BACKWARD", dt);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            appCamera.ProcessKeyboard("LEFT", dt);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            appCamera.ProcessKeyboard("RIGHT", dt);
    }

    void enterCar(Example::Car* car)
    {
        /*car->setDriverDoorOpen(true);*/

        if (!car) return;

        std::cout << "🚗 Entering car..." << std::endl;

        currentCar = car;
        isInCar = true;
        currentCar->setDriverDoorOpen(true);

        // حفظ الموقع الحالي للعودة إليه لاحقاً
        savedPosition = appCamera.Position;
        savedYaw = appCamera.Yaw;
        savedPitch = appCamera.Pitch;

        // الانتقال لمقعد السائق
        appCamera.Position = car->getDriverSeatPosition();

        // توجيه الكاميرا للأمام (اتجاه السيارة)
        glm::vec3 viewDir = car->getDriverViewDirection();
        appCamera.Yaw = glm::degrees(atan2(viewDir.z, viewDir.x));
        appCamera.Pitch = -5.0f;  // النظر للأمام قليلاً للأسفل

        std::cout << "✅ Now sitting in driver seat!" << std::endl;
        std::cout << "   Press E to exit the car" << std::endl;
    }

    void exitCar()
    {
        if (!isInCar || !currentCar) return;

        std::cout << "🚶 Exiting car..." << std::endl;

        

        // العودة للموقع المحفوظ
        appCamera.Position = savedPosition;
        appCamera.Yaw = savedYaw;
        appCamera.Pitch = savedPitch;

        if (currentCar)
        {
            currentCar->setThrottle(0.0f);
            currentCar->setDriverDoorOpen(false);
        }

        isInCar = false;
        currentCar = nullptr;

        std::cout << "✅ Exited car!" << std::endl;
    }

    void updateWindowTitle()
    {
        std::string title = "Car Showroom";

        if (isInCar)
        {
            title += " | 🚗 Inside Car - Press E to Exit";
        }
        else if (nearCar)
        {
            title += " | 💡 Press E to Enter Car";
        }
        else
        {
            title += " | Walking";
        }

        glfwSetWindowTitle(window, title.c_str());
    }
};

// مؤشر عالمي للوصول إلى التطبيق
Application* g_AppInstance = nullptr;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static float lastX = 640, lastY = 360;
    static bool firstMouse = true;

    if (firstMouse)
    {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos;

    lastX = (float)xpos;
    lastY = (float)ypos;

    if (g_AppInstance)
    {
        // ✅ تقييد الحركة داخل السيارة
        if (g_AppInstance->isInCar)
        {
            // السماح بنظرة محدودة داخل السيارة
            xoffset *= 0.5f;  // تقليل حساسية الدوران
            yoffset *= 0.5f;
        }

        g_AppInstance->appCamera.ProcessMouseMovement(xoffset, yoffset);
    }
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // إعدادات OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Application app;
    g_AppInstance = &app;

    app.window = glfwCreateWindow(1280, 720, "Interactive Car Showroom - OpenGL", NULL, NULL);
    if (!app.window)
    {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(app.window);
    glfwSetCursorPosCallback(app.window, mouse_callback);
    glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGL())
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    app.onInit();

    while (!glfwWindowShouldClose(app.window))
    {
        app.onUpdate();

        glfwSwapBuffers(app.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}