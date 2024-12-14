#pragma once
#include "Core/Window.h"
#include "Rendering/Camera/Camera.h"
#include "World/Scene/Scene.h"
#include "Physics/Include.h"
#include "Config.h"

#include <thread>

namespace Verlet
{
	class Engine
	{
	public:
		Engine();

		// This is purely a stylistic choice over using the constructor
		void Init();

		// Starts our application cycle
		void Run();

		// Closes our application on the next resumption cycle
		inline void Close() { m_close = true; };

		inline Scene* GetScene() { return m_scene; };

#if DEBUG_FEATURES
		void DrawWidgets();
#endif

		// Threaded functions
		void PhysicsRun();

		// Functions that are called each resumption cycle
		void ProcessInput();
		void PollEvents();

		void FrameStart();
		void PreRender();
		void Render();
		void PostRender();
		void FrameEnd();

	private:
		bool m_ready = false;
		bool m_close = false;

		double m_lastTime = 0.0f;

		Window* m_window = nullptr;
		Camera* m_camera = nullptr;
		Scene* m_scene = nullptr;
		Physics::phSystem* m_physicsSystem = nullptr;

		std::thread m_physicsThread;
	};

	extern Verlet::Engine* CurrentEngine;
}
