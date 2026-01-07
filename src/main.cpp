#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/io.hpp>

#include <string>
#include <vector>
#include <iostream>

#include "basic-shape.hpp"
#include "textured-shape.hpp"

namespace Colors
{
	glm::vec3 red{ 1.f, 0.f, 0.f };
	glm::vec3 green{ 0.f, 1.f, 0.f };
	glm::vec3 blue{ 0.f, 0.f, 1.f };

	glm::vec3 white{ 1.f, 1.f, 1.f };
	glm::vec3 black{ 0.f, 0.f, 0.f };

	glm::vec3 yellow{ 1.f, 1.f, 0.f };
	glm::vec3 magenta{ 1.f, 0.f, 1.f };
	glm::vec3 cyan{ 0.f, 1.f, 1.f };
}

class Application
{
public:
	Example::BasicShape shape;

	glm::mat4 transform = { 1.0f };
	glm::mat4 camera = { 1.0f };

	void onInit()
	{
		using namespace Colors;

		shape = Example::BasicShape({
			{ {0.0f, 0.0f, 0.8f }, red },
			{ {0.0f, 0.5f, -0.5f}, green },
			{ {0.0f, -0.5f, -0.5f}, blue },
			});
	}

	void onUpdate(float t, float dt)
	{
		transform = glm::identity<glm::mat4>();
		transform = glm::rotate(transform, glm::radians(t * 60.0f), { 0.0f, 0.0f, 1.0f });
		transform = glm::translate(transform, { 0.0f, 0.0f, glm::sin(glm::radians(t * 45.0f)) * 0.1f - 0.1f });
	}

	void onDraw(float t, float dt)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shape.render(transform, camera);
	}

	void onExit()
	{
	}

	void updateCamera(float t, float dt)
	{
		float distance = 2.4f;

		glm::mat4 view = glm::lookAt(
			glm::vec3{ distance, 0.0f, 0.0f },
			glm::vec3{ 0.0f, 0.0f, 0.0f },
			glm::vec3{ 0.0f, 0.0f, 1.0f });

		int width, height;
		glfwGetWindowSize(window, &width, &height);
		float aspect = static_cast<float>(width) / static_cast<float>(height);

		glm::mat4 projection = glm::perspective(
			glm::radians(45.0f), aspect, 0.1f, 1000.0f);

		camera = projection * view;
	}

	GLFWwindow* window;

	void initializeWindowAndGraphics(int width, int height, const char* windowTitle)
	{
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

		window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			std::exit(-1);
		}

		glfwMakeContextCurrent(window);

		glfwSwapInterval(1);

		if (!gladLoadGL()) {
			std::cerr << "Failed to initialize GLAD (OpenGL)!" << std::endl;
			std::exit(-1);
		}

		glEnable(GL_DEPTH_TEST);
	}

	float lastFrameTime;

	void runEventLoop()
	{
		onInit();

		glfwSetFramebufferSizeCallback(window, Application::glfw_resize_callback);

		while (!glfwWindowShouldClose(window))
		{
			float t = static_cast<float>(glfwGetTime());
			float dt = t - lastFrameTime;
			lastFrameTime = t;

			updateCamera(t, dt);

			onUpdate(t, dt);
			onDraw(t, dt);

			glfwSwapBuffers(window);

			glfwPollEvents();
		}

		onExit();
	}

	static void glfw_resize_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
};

int main(int argc, char** args)
{
	if (!glfwInit())
		return -1;

	Application application;
	application.initializeWindowAndGraphics(
		800, 600, "OpenGL Application (GLFW, VS)");
	application.runEventLoop();

	glfwTerminate();
}
