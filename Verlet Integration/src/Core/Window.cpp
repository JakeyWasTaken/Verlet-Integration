#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Config.h"
#include "Window.h"
#include "Log.h"
#include "Input.h"

void framebuffer_size_callback(GLFWwindow* _, int width, int height);


namespace Verlet
{
	Window* CurrentWindow = nullptr;

	Window::Window()
	{
		if (!glfwInit())
		{
			VT_FATAL("Failed to init GLFW");
			exit(-1);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CONF_GL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CONF_GL_VERSION_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, CONF_GL_PROFILE);
		// TODO: enable multisampling

		GLFWwindow* window = glfwCreateWindow(CONF_WINDOW_WIDTH, CONF_WINDOW_HEIGHT, CONF_WINDOW_TITLE, NULL, NULL);
		if (window == NULL)
		{
			VT_FATAL("Failed to create GLFW window");
			glfwTerminate();
			exit(-1);
		}

		glfwMakeContextCurrent(window);
		m_glfwWindow = window;
		CurrentWindow = this;

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			VT_FATAL("Failed to initialize GLAD");
			glfwTerminate();
			exit(-1);
		}

		glViewport(0, 0, CONF_WINDOW_WIDTH, CONF_WINDOW_HEIGHT);

		// Assign events here
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		Init();
	}

	void Window::Init()
	{
		Input::Init(m_glfwWindow);

		VT_INFO("Initialized window");
	}

	// Events
	void Window::InputStep()
	{
		if (GET_KEY_STATE(GLFW_KEY_ESCAPE) == KEY_PRESS)
			glfwSetWindowShouldClose(m_glfwWindow, 1);
	}

	void Window::WindowSizeChanged(int width, int height)
	{
		m_width = width;
		m_height = height;

		glViewport(0, 0, width, height);
	}
}

void framebuffer_size_callback(GLFWwindow* _, int width, int height)
{
	if (!Verlet::CurrentWindow)
		return;

	Verlet::CurrentWindow->WindowSizeChanged(width, height);
}