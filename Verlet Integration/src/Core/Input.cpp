#include "Input.h"
#include "Log.h"

namespace Verlet
{
	GLFWwindow* Input::m_glfwWindow;

	void Input::Init(GLFWwindow* glfwWindow)
	{
		m_glfwWindow = glfwWindow;

		VT_INFO("Initialized input");
	}

	int Input::GetKeyState(int Key)
	{
		return glfwGetKey(m_glfwWindow, Key);
	}

	int Input::GetMouseButtonState(int MouseButton)
	{
		return glfwGetMouseButton(m_glfwWindow, MouseButton);
	}

	void Input::GetMousePosition(double* mouseX, double* mouseY)
	{
		glfwGetCursorPos(m_glfwWindow, mouseX, mouseY);
	}
}