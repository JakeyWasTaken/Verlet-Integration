#include "Engine.h"
#include "Core/Log.h"
#include "Core/Time.h"
#include "Rendering/Debug/DebugDraw.h"
#include "World/Lighting/Lighting.h"
#include "Rendering/Debug/DebugImGui.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glad/glad.h"

#include "Physics/Constraints/DistanceConstraint.h"

#define DISTANCE_CONSTRAINT(p0, p1) m_physicsSystem->AddConstraint(new Verlet::Physics::DistanceConstraint(p0, p1, 1.f))

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
		m_physicsSystem = new Physics::System();

		const uint32_t size = 10;
		const float pointSpacing = 0.25f;
		Physics::Point* points[size][size] = {};

		glm::vec3 positionStart = glm::vec3(0.0f, 25.0f, 0.0f);
		for (uint32_t i = 0; i < size; i++)
		{
			for (uint32_t j = 0; j < size; j++)
			{
				Physics::Point* point = new Physics::Point(positionStart + glm::vec3(i, j, 0) * pointSpacing, 1.0f);
				points[i][j] = point;

				if (j == 0)
					point->SetMass(0.0f);

				m_physicsSystem->AddPoint(point);

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

		/*Physics::Point* frozenPoint = new Physics::Point(glm::vec3(0.0f, 10.0f, 0.0f), 0.0f);
		Physics::Point* p = new Physics::Point(glm::vec3(0.0f, 5.0f, 0.0f), 1.5f);

		m_physicsSystem->AddPoint(frozenPoint);
		m_physicsSystem->AddPoint(p);

		Physics::DistanceConstraint* c = new Physics::DistanceConstraint(p, frozenPoint);
		c->compliance = 0.0001f;
		m_physicsSystem->AddConstraint(c);*/
		
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


			FrameStart();
			PhysicsStep();
			PreRender();
			Render();
			PostRender();
			FrameEnd();

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

	void Engine::FrameStart()
	{
		dbgImGui::Prepare();
	}

	void Engine::PhysicsStep()
	{
		m_physicsSystem->Update(Time::DeltaTime);
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
		m_physicsSystem->DrawWidgets();

		grDebugDraw::Draw(m_camera);
		dbgImGui::Render();
	}

	void Engine::PostRender() {}

	void Engine::FrameEnd()
	{
		grDebugDraw::ClearDrawList();
	}
}