#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <string>

#include "basic-shape.hpp"
#include "textured-shape.hpp"
#include "../Camera.hpp"
#include "../Showroom.hpp"
#include "../Car.hpp"
#include "../grass.hpp"
#include "../tree.hpp"


// تابع تحميل الصور
GLuint loadTexture(const char* path) {
    int w, h, ch;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &w, &h, &ch, 0);
    if (!data) return 0;
    GLuint id; glGenTextures(1, &id); glBindTexture(GL_TEXTURE_2D, id);
    GLenum f = (ch == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, f, w, h, 0, f, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data); return id;
}

class Application
{
public:
    GLFWwindow* window;
    float lastFrameTime = 0.0f;

    Example::Camera appCamera;
    Example::Showroom showroom;
    Example::Grass grassField;
    Example::Tree treeField; 

    bool isInCar = false;
    Example::Car* currentCar = nullptr;
    glm::vec3 savedPosition;
    float savedYaw, savedPitch;

    bool eKeyWasPressed = false;
    bool nearCar = false;

    Application() : appCamera(glm::vec3(0.0f, 2.5f, 75.0f)), window(nullptr) {}

    void onInit()
    {
        Example::BasicShape::compileShapeShader();
        showroom.init();

        GLuint grassTex = loadTexture(R"(C:\Projects\Computer-Graphics-Project\resources\photos\grass.png)");
        grassField.initGrassField(grassTex, 200.0f, 0.3f);
        GLuint treeTexMini = loadTexture(R"(C:\Projects\Computer-Graphics-Project\resources\photos\mini-tree.png)");
        GLuint treeTexHuge = loadTexture(R"(C:\Projects\Computer-Graphics-Project\resources\photos\huge-tree.png)");
        treeField.initTrees(treeTexMini, treeTexHuge);

        std::cout << "Application Initialized. ESC to Exit." << std::endl;
    }

    void onUpdate()
    {
        float t = (float)glfwGetTime();
        float dt = t - lastFrameTime;
        lastFrameTime = t;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        if (!isInCar) {
            glm::vec3 oldPos = appCamera.Position;
            handleMovement(dt);
            appCamera.Position = showroom.checkCollision(oldPos, appCamera.Position);
            appCamera.Position.y = 2.5f;

            Example::Car* nearbyCar = showroom.findNearestCar(appCamera.Position);
            nearCar = (nearbyCar != nullptr);

            bool ePressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
            if (ePressed && !eKeyWasPressed && nearbyCar != nullptr) enterCar(nearbyCar);
            eKeyWasPressed = ePressed;

            static bool fKeyWasPressed = false;
            bool fPressed = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
            if (fPressed && !fKeyWasPressed) showroom.toggleNearestRoomDoor(appCamera.Position);
            fKeyWasPressed = fPressed;
        }
        else {
            if (!currentCar) { isInCar = false; return; }
            bool ePressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
            if (ePressed && !eKeyWasPressed) { exitCar(); eKeyWasPressed = ePressed; return; }
            eKeyWasPressed = ePressed;

            float tInput = 0.0f;
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) tInput += 1.0f;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) tInput -= 1.0f;
            currentCar->setThrottle(tInput);

            float sInput = 0.0f;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) sInput -= 1.0f;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) sInput += 1.0f;
            currentCar->setSteer(sInput);
        }

        showroom.update(dt);
        if (isInCar && currentCar) appCamera.Position = currentCar->getDriverSeatPosition();
        showroom.lighting.setViewPosition(appCamera.Position);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        float aspectRatio = (float)width / (float)height;
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspectRatio, 0.1f, 400.0f);
        glm::mat4 viewProj = projection * appCamera.GetViewMatrix();

        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        showroom.renderAll(viewProj);
        // اعدادات الشفافية للعشب والشجر
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        // رسم العشب
        grassField.draw(viewProj);

        // رسم الشجر
        treeField.draw(viewProj);

        glDepthMask(GL_TRUE);

        updateWindowTitle();
    }

private:
    void handleMovement(float dt) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) appCamera.ProcessKeyboard("FORWARD", dt);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) appCamera.ProcessKeyboard("BACKWARD", dt);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) appCamera.ProcessKeyboard("LEFT", dt);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) appCamera.ProcessKeyboard("RIGHT", dt);
    }

    void enterCar(Example::Car* car) {
        currentCar = car; isInCar = true;
        currentCar->setDriverDoorOpen(true);
        savedPosition = appCamera.Position; savedYaw = appCamera.Yaw; savedPitch = appCamera.Pitch;
        appCamera.Position = car->getDriverSeatPosition();
        appCamera.Pitch = -5.0f;
    }

    void exitCar() {
        appCamera.Position = savedPosition; appCamera.Yaw = savedYaw; appCamera.Pitch = savedPitch;
        currentCar->setThrottle(0.0f); currentCar->setDriverDoorOpen(false);
        isInCar = false; currentCar = nullptr;
    }

    void updateWindowTitle() {
        std::string title = "Car Showroom | ESC to Exit";
        if (isInCar) title += " | Driving Mode";
        glfwSetWindowTitle(window, title.c_str());
    }
};

Application* g_AppInstance = nullptr;
void mouse_callback(GLFWwindow* w, double x, double y) {
    static float lx = 640, ly = 360, fm = true;
    if (fm) { lx = (float)x; ly = (float)y; fm = false; }
    float ox = (float)x - lx; float oy = ly - (float)y;
    lx = (float)x; ly = (float)y;
    if (g_AppInstance) g_AppInstance->appCamera.ProcessMouseMovement(ox, oy);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Application app;
    g_AppInstance = &app;
    app.window = glfwCreateWindow(1280, 720, "Car Showroom", NULL, NULL);
    glfwMakeContextCurrent(app.window);
    glfwSetCursorPosCallback(app.window, mouse_callback);
    glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);
    app.onInit();

    while (!glfwWindowShouldClose(app.window)) {
        app.onUpdate();
        glfwSwapBuffers(app.window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}