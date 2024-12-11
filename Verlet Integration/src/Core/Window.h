#pragma once
#include <GLFW/glfw3.h>
#include <stdfloat>


namespace Verlet
{
	class Window
	{
	public:
		Window();

		inline GLFWwindow* GetGLFWWindow() { return m_glfwWindow; };
		inline int GetWidth() { return m_width; };
		inline int GetHeight() { return m_height; };

		void Init();

		// Events
		void InputStep();
		void WindowSizeChanged(int width, int height);
	private:
		int m_width;
		int m_height;

		GLFWwindow* m_glfwWindow;
	};

	extern Verlet::Window* CurrentWindow;
}
