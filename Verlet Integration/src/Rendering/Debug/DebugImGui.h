#pragma once
#include "Core/Window.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace Verlet
{
	class dbgImGui
	{
	public:
		static void Init(Window* window);

		static void Prepare();
		static void Render();

		static void Cleanup();
	};
}

