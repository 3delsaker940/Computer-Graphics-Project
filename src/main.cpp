#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "textured-shape.hpp"
#include "../grass.hpp"
#include "../tree.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

#include "basic-shape.hpp"
#include "../Camera.hpp"
#include "../Audio.hpp"
#include "../Showroom.hpp"
#include "../Car.hpp"

GLuint loadTexture(const char* path)
{
    int w, h, ch;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &w, &h, &ch, 0);
    if (!data) return 0;

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    GLenum format = (ch == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return id;
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

        std::cout << "========================================" << std::endl;
        std::cout << "   Interactive Car Showroom" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "  W/A/S/D - Move" << std::endl;
        std::cout << "  Mouse   - Look around" << std::endl;
        std::cout << "  E       - Enter/Exit car" << std::endl;
        std::cout << "  ESC     - Quit" << std::endl;
        std::cout << "========================================" << std::endl;
        GLuint grassTex = loadTexture("resources/photos/grass.png");
        grassField.initGrassField(grassTex, 200.0f, 0.3f);

        GLuint treeTexMini = loadTexture("resources/photos/mini-tree.png");
        GLuint treeTexHuge = loadTexture("resources/photos/huge-tree.png");
        treeField.initTrees(treeTexMini, treeTexHuge);

    }
    void onUpdate()
    {
        float t = (float)glfwGetTime();
        float dt = t - lastFrameTime;
        lastFrameTime = t;

                        
        if (!isInCar)
        {
            glm::vec3 oldPos = appCamera.Position;
            handleMovement(dt);
            appCamera.Position = showroom.checkCollision(oldPos, appCamera.Position);
            appCamera.Position.y = 2.5f;

            Example::Car* nearbyCar = showroom.findNearestCar(appCamera.Position);
            nearCar = (nearbyCar != nullptr);

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

                        
            bool ePressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
            if (ePressed && !eKeyWasPressed)
            {
                exitCar();
                eKeyWasPressed = ePressed;
                return;           
            }
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

                                        {
          

            const float buildingHalfSize = 70.0f; 
            bool isOutside =
                (appCamera.Position.x < -buildingHalfSize) ||
                (appCamera.Position.x > buildingHalfSize) ||
                (appCamera.Position.z < -buildingHalfSize) ||
                (appCamera.Position.z > buildingHalfSize);

            static bool wasOutside = false;

            if (isOutside && !wasOutside)
            {
                              
                audioOutdoorStart();
                audioIndoorStop();
            }
            else if (!isOutside && wasOutside)
            {
                               
                audioOutdoorStop();
                audioIndoorStart();
            }

            wasOutside = isOutside;
        }

                if (isInCar && currentCar)
        {
            appCamera.Position = currentCar->getDriverSeatPosition();
        }

               

                if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

               
                showroom.lighting.setViewPosition(appCamera.Position);

                static bool lKeyWasPressed = false;
        bool lPressed = glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS;
        if (lPressed && !lKeyWasPressed)
        {
            showroom.toggleLights();
        }
        lKeyWasPressed = lPressed;

                        
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        float aspectRatio = (float)width / (float)height;
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspectRatio, 0.1f, 300.0f);
        glm::mat4 viewProj = projection * appCamera.GetViewMatrix();

                if (isInCar)
            glClearColor(0.02f, 0.02f, 0.03f, 1.0f);   
                else
            glClearColor(0.05f, 0.05f, 0.1f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        showroom.renderAll(viewProj);

                glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        grassField.draw(viewProj);
        treeField.draw(viewProj);

        glDepthMask(GL_TRUE);

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

        std::cout << " Entering car..." << std::endl;

        audioPlay("assets/sounds/engine_start.flac");
        currentCar = car;
        isInCar = true;
        currentCar->setDriverDoorOpen(true);

                savedPosition = appCamera.Position;
        savedYaw = appCamera.Yaw;
        savedPitch = appCamera.Pitch;

                appCamera.Position = car->getDriverSeatPosition();

                glm::vec3 viewDir = car->getDriverViewDirection();
        appCamera.Yaw = glm::degrees(atan2(viewDir.z, viewDir.x));
        appCamera.Pitch = -5.0f;  
        std::cout << " Now sitting in driver seat!" << std::endl;
        std::cout << "   Press E to exit the car" << std::endl;
    }

    void exitCar()
    {
        if (!isInCar || !currentCar) return;

        std::cout << " Exiting car..." << std::endl;



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

        std::cout << " Exited car!" << std::endl;
    }

    void updateWindowTitle()
    {
        std::string title = "Car Showroom";

        if (isInCar)
        {
            title += " |  Inside Car - Press E to Exit";
        }
        else if (nearCar)
        {
            title += " |  Press E to Enter Car";
        }
        else
        {
            title += " | Walking";
        }

        glfwSetWindowTitle(window, title.c_str());
    }
};

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
                if (g_AppInstance->isInCar)
        {
                        xoffset *= 0.5f;              yoffset *= 0.5f;
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
    audioInit();

    while (!glfwWindowShouldClose(app.window))
    {
        app.onUpdate();

        glfwSwapBuffers(app.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}