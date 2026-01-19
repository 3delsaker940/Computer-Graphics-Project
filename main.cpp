#include <glad/glad.h>
#include <GLFW/glfw3.h>

// مكتبات GLM الأساسية
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>

// ملفات المشروع
#include "basic-shape.hpp"
#include "Camera.hpp"

// تعريف الكائن بشكل عالمي لضمان وصول الـ callback إليه
Camera camera(glm::vec3(2.4f, 0.0f, 0.5f));
float lastX = 400, lastY = 300;
bool firstMouse = true;

// دالة تحريك الماوس
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

class Application
{
public:
    Example::BasicShape shape;
    glm::mat4 transform = glm::mat4(1.0f);
    glm::mat4 cameraMat = glm::mat4(1.0f);
    GLFWwindow *window;
    float lastFrameTime = 0.0f;

    void onInit()
    {
        Example::BasicShape::compileShapeShader();
        // إنشاء مثلث ملون للتجربة كما في مشروعك الأصلي
        shape = Example::BasicShape({
            {{0.0f, 0.0f, 0.8f}, {1.0f, 0.0f, 0.0f}},
            {{0.0f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.0f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
        });
    }

    void onUpdate(float t, float dt)
    {
        // معالجة حركة الكاميرا
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard("FORWARD", dt);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard("BACKWARD", dt);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard("LEFT", dt);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard("RIGHT", dt);

        // تحديث مصفوفة الكاميرا (Projection * View)
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        float aspect = static_cast<float>(width) / (float)height;
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        cameraMat = projection * camera.GetViewMatrix();

        // تدوير الشكل قليلاً
        transform = glm::rotate(glm::mat4(1.0f), glm::radians(t * 30.0f), {0.0f, 0.0f, 1.0f});
    }

    void onDraw()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // استدعاء دالة render من كلاس BasicShape
        shape.render(transform, cameraMat);
    }
};

int main()
{
    if (!glfwInit())
        return -1;

    Application app;
    // إعدادات النافذة
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    app.window = glfwCreateWindow(800, 600, "Car Showroom - Step 1", NULL, NULL);
    if (!app.window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(app.window);
    if (!gladLoadGL())
        return -1; // تحميل مكتبة GLAD
    glEnable(GL_DEPTH_TEST);

    // تفعيل الماوس
    glfwSetCursorPosCallback(app.window, mouse_callback);
    glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    app.onInit();

    while (!glfwWindowShouldClose(app.window))
    {
        float t = static_cast<float>(glfwGetTime());
        float dt = t - app.lastFrameTime;
        app.lastFrameTime = t;

        app.onUpdate(t, dt);
        app.onDraw();

        glfwSwapBuffers(app.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}