#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

// تأكد من صحة المسارات بناءً على بنية مجلداتك
#include "basic-shape.hpp"
#include "../Camera.hpp" // إذا كان ملف الكاميرا في المجلد الأب
#include "../Showroom.hpp"

class Application
{
public:
    GLFWwindow *window;
    float lastFrameTime = 0.0f;

    // تعريف الكائنات داخل الكلاس لضمان تهيئتها بعد Glad
    Example::Camera appCamera;
    Example::Showroom showroom;

    // Constructor لتهيئة الكاميرا في موقع افتراضي
    Application() : appCamera(glm::vec3(0.0f, 1.7f, 10.0f)), window(nullptr) {}

    void onInit()
    {
        // تهيئة الشيدرز وبناء الصالات
        Example::BasicShape::compileShapeShader();
        showroom.init();
        std::cout << "Showroom Initialized Successfully!" << std::endl;
    }

    void onUpdate()
    {
        // حساب الوقت المستغرق بين الإطارات (Delta Time)
        float t = (float)glfwGetTime();
        float dt = t - lastFrameTime;
        lastFrameTime = t;

        // 1. معالجة المدخلات (الحركة)
        glm::vec3 oldPos = appCamera.Position;
        handleInput(dt);

        // 2. تطبيق نظام التصادم (Collision)
        // نمرر الموقع القديم والجديد لضمان عدم اختراق الجدران
        appCamera.Position = showroom.checkCollision(oldPos, appCamera.Position);

        // تثبيت ارتفاع الكاميرا (محاكاة مشي الإنسان)
        appCamera.Position.y = 1.7f;

        // 3. إعداد مصفوفات العرض والإسقاط
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        float aspectRatio = (float)width / (float)height;
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 300.0f);
        glm::mat4 viewProj = projection * appCamera.GetViewMatrix();

        // 4. عمليات الرسم
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // لون خلفية ليلي هادئ
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        showroom.renderAll(viewProj);
    }

private:
    void handleInput(float dt)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            appCamera.ProcessKeyboard("FORWARD", dt);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            appCamera.ProcessKeyboard("BACKWARD", dt);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            appCamera.ProcessKeyboard("LEFT", dt);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            appCamera.ProcessKeyboard("RIGHT", dt);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
};

// مؤشر عالمي للوصول إلى التطبيق من داخل الكوالباك
Application *g_AppInstance = nullptr;

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
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
        g_AppInstance->appCamera.ProcessMouseMovement(xoffset, yoffset);
    }
}

int main()
{
    // 1. تهيئة GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 2. إنشاء كائن التطبيق
    Application app;
    g_AppInstance = &app;

    // 3. إعداد النافذة
    app.window = glfwCreateWindow(1280, 720, "Interative Car Showroom - OpenGL", NULL, NULL);
    if (!app.window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(app.window);
    glfwSetCursorPosCallback(app.window, mouse_callback);
    glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // 4. تحميل وظائف OpenGL بواسطة GLAD
    if (!gladLoadGL())
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 5. إعدادات OpenGL العامة
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 6.تهيئة الموارد (الصالات، الشيدرز)
    app.onInit();

    // 7. حلقة البرنامج الأساسية
    while (!glfwWindowShouldClose(app.window))
    {
        app.onUpdate();

        glfwSwapBuffers(app.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}