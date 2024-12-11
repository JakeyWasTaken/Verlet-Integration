#pragma once
#include "GLFW/glfw3.h"

#define KEY_PRESS GLFW_PRESS
#define KEY_RELEASE GLFW_RELEASE

#define MOUSE_PRESS GLFW_PRESS
#define MOUSE_RELEASE GLFW_RELEASE

#define LEFT_MOUSE_BUTTON GLFW_MOUSE_BUTTON_1
#define RIGHT_MOUSE_BUTTON GLFW_MOUSE_BUTTON_2

namespace Verlet
{
	class Input
	{
	public:
		static void Init(GLFWwindow* glfwWindow);
		static int GetKeyState(int key);
		static int GetMouseButtonState(int mouseButton);
		static void GetMousePosition(double* mouseX, double* mouseY);

		inline static GLFWwindow* GetGLFWWindow() { return m_glfwWindow; };

	private:
		static GLFWwindow* m_glfwWindow;
	};
}

#define GET_KEY_STATE(key) ::Verlet::Input::GetKeyState(key)
#define GET_MOUSE_BUTTON_STATE(mouseButton) ::Verlet::Input::GetMouseButtonState(mouseButton)
#define GET_MOUSE_POSITION(mouseX, mouseY) ::Verlet::Input::GetMousePosition(mouseX, mouseY)