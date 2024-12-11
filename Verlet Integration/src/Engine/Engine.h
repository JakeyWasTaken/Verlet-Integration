#pragma once
#include "Core/Window.h"
#include "Rendering/Camera/Camera.h"
#include "World/Scene/Scene.h"

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

		// Functions that are called each resumption cycle
		void ProcessInput();

		void PreRender();
		void Render();
		void PostRender();

	private:
		bool m_ready = false;
		bool m_close = false;

		double m_lastTime = 0;

		Window* m_window = nullptr;
		Camera* m_camera = nullptr;
		Scene* m_scene = nullptr;
	};

	extern Verlet::Engine* CurrentEngine;
}
