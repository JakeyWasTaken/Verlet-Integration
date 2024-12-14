#include "Engine.h"
#include "Core/Log.h"
#include "Core/Time.h"
#include "Rendering/Debug/DebugDraw.h"
#include "World/Lighting/Lighting.h"
#include "Rendering/Debug/DebugImGui.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glad/glad.h"

#define DISTANCE_CONSTRAINT(p0, p1) cloth->AddConstraint(new Verlet::Physics::DistanceConstraint(p0, p1, 0.0001f))

void ImGuiRightAlignText(const char* text)
{
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(text).x
		- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
}

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
		m_physicsSystem = new Physics::phSystem();

		// TEMPORARY START

		Physics::phObject* cloth = new Physics::phObject();

		const uint32_t size = 100;
		const float pointSpacing = 0.25f;
		Physics::phPoint* points[size][size] = {};

		glm::vec3 positionStart = glm::vec3(0.0f, 25.0f, 0.0f);
		for (uint32_t i = 0; i < size; i++)
		{
			for (uint32_t j = 0; j < size; j++)
			{
				Physics::phPoint* point = new Physics::phPoint(positionStart + glm::vec3(i, j, 0) * pointSpacing, 5.0f);
				points[i][j] = point;

				if (j == 0)
					point->SetMass(0.0f);

				cloth->AddPoint(point);

				// Point to the left
				if (i > 0)
					DISTANCE_CONSTRAINT(points[i - 1][j], point);

				// Point above to left
				if (i > 0 && j > 0)
					DISTANCE_CONSTRAINT(points[i - 1][j - 1], point);

				// Point above
				if (j > 0)
					DISTANCE_CONSTRAINT(points[i][j - 1], point);
			}
		}

		m_physicsSystem->AddObject(cloth);

		/*Physics::Point* frozenPoint = new Physics::Point(glm::vec3(0.0f, 10.0f, 0.0f), 0.0f);
		Physics::Point* p = new Physics::Point(glm::vec3(0.0f, 5.0f, 0.0f), 1.5f);

		m_physicsSystem->AddPoint(frozenPoint);
		m_physicsSystem->AddPoint(p);

		Physics::DistanceConstraint* c = new Physics::DistanceConstraint(p, frozenPoint);
		c->compliance = 0.0001f;
		m_physicsSystem->AddConstraint(c);*/

		// TEMPORARY END
		
		// This must get initialized after the window since the window sets-up our context
		grDebugDraw::Init();
		dbgImGui::Init(m_window);

		m_ready = true;

		// Start threads
		m_physicsThread = std::thread(&Engine::PhysicsRun, this);

		m_physicsThread.detach();
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

		// glfwWindowShouldClose(glfwWindow)
		while (!glfwWindowShouldClose(glfwWindow) && !m_close)
		{
			// Set our time, delta time and increment our frame counter
			double now = glfwGetTime();
			double deltaTime = now - m_lastTime;
			m_lastTime = now;

			Time::Time = now;
			Time::DeltaTime = deltaTime;
			Time::Frame += 1;

			PollEvents();

			ProcessInput();

			FrameStart();
			//PhysicsStep();
			PreRender();
			Render();
			PostRender();
			FrameEnd();

			glfwSwapBuffers(glfwWindow);
		}

		// Setting this will terminate the physics thread
		m_close = true;

		dbgImGui::Cleanup();

		// After finishing running terminate glfw
		glfwDestroyWindow(glfwWindow);
		glfwTerminate();
	}
	
	void Engine::PollEvents()
	{
		glfwPollEvents();
		//glfwWaitEvents();
	}

	void Engine::ProcessInput()
	{
		m_window->InputStep();
		m_camera->ProcessKeyboard();
		m_camera->ProcessMouse();
	}

	void Engine::FrameStart()
	{
		dbgImGui::Prepare();
	}

#if DEBUG_FEATURES
	void Engine::DrawWidgets()
	{
		if (ImGui::BeginMainMenuBar())
		{
			const char* RightInfoFormat = "Verlet Integration (Build: %s / %i) (Framerate: %.3f ms (%.2f fps) )";
			char RightInfo[512];
			sprintf_s(RightInfo, RightInfoFormat, CONF_BUILD_TYPE, BUILD_COUNT, Time::DeltaTime, 1 / Time::DeltaTime);

			ImGuiRightAlignText(RightInfo);
			ImGui::Text(RightInfo);

			ImGui::EndMainMenuBar();
		}
	}
#endif

	void Engine::PhysicsRun()
	{
		static double lastTime = glfwGetTime();

		while (!m_close)
		{
			double time = glfwGetTime();
			double delta = time - lastTime;

			m_physicsSystem->IncrementAccumulator(delta);

			while (m_physicsSystem->ReadyToUpdate())
			{
				m_physicsSystem->IncrementAccumulator(-m_physicsSystem->settings.updateRate);
				m_physicsSystem->Update();
			}
		}

		// Terminate this thread
		std::terminate();
	}

	void Engine::PreRender()
	{
		glClearColor(Lighting::SkyColor.r, Lighting::SkyColor.g, Lighting::SkyColor.b, 1.0f);
		glEnable(GL_DEPTH_TEST);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Engine::Render()
	{
		m_scene->Draw(m_camera);
#if DEBUG_FEATURES
		DrawWidgets();
		m_physicsSystem->DrawDebug();
#endif

		grDebugDraw::Draw(m_camera);

#if SHOW_DEARIMGUI_DEMO
		ImGui::ShowDemoWindow();
#endif
		dbgImGui::Render();
	}

	void Engine::PostRender() {}

	void Engine::FrameEnd()
	{
		grDebugDraw::ClearDrawList();
	}
}