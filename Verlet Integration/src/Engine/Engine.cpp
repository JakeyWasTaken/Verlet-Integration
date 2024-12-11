#include "Engine.h"
#include "Core/Log.h"
#include "Core/Time.h"
#include "Rendering/Debug/DebugDraw.h"
#include "World/Lighting/Lighting.h"
#include "Rendering/Debug/DebugImGui.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glad/glad.h"

namespace Verlet
{
	Engine* CurrentEngine = nullptr;

	Engine::Engine()
	{
		CurrentEngine = this;
	}

	void Engine::Init()
	{
		Log::Init();

		m_window = new Window();
		m_camera = new Camera(glm::vec3(0.0f));
		m_scene = new Scene();
		
		// This must get initialized after the window since the window sets-up our context
		grDebugDraw::Init();
		dbgImGui::Init(m_window);

		m_ready = true;
	}

	void Engine::Run()
	{
		if (!m_ready)
		{
			VT_ERROR("Engine::Run was called before being initialized");
			__debugbreak();
			exit(-1);
		}

		GLFWwindow* glfwWindow = m_window->GetGLFWWindow();

		while (!glfwWindowShouldClose(glfwWindow) && !m_close)
		{
			// Set our time, delta time and increment our frame counter
			double now = glfwGetTime();
			double deltaTime = now - m_lastTime;
			m_lastTime = now;

			Time::Time = now;
			Time::DeltaTime = deltaTime;
			Time::Frame += 1;

			glfwPollEvents();
			ProcessInput();

			PreRender();
			Render();
			PostRender();

			glfwSwapBuffers(glfwWindow);
		}

		dbgImGui::Cleanup();

		// After finishing running terminate glfw
		glfwDestroyWindow(glfwWindow);
		glfwTerminate();
	}
	
	void Engine::ProcessInput()
	{
		m_window->InputStep();
		m_camera->ProcessKeyboard();
		m_camera->ProcessMouse();
	}

	void Engine::PreRender()
	{
		glClearColor(Lighting::SkyColor.r, Lighting::SkyColor.g, Lighting::SkyColor.b, 1.0f);
		glEnable(GL_DEPTH_TEST);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		dbgImGui::PreRender();
	}

	void Engine::Render()
	{
		m_scene->Draw(m_camera);
		grDebugDraw::Draw(m_camera);

		ImGui::ShowDemoWindow();
		dbgImGui::Render();
	}

	void Engine::PostRender()
	{
		grDebugDraw::ClearDrawList();
	}
}