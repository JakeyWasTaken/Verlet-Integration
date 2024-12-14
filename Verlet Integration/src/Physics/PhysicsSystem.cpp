#include "PhysicsSystem.h"
#include "Rendering/Debug/DebugImGui.h"
#include "Core/Random.h"
#include "Core/Vector.h"
#include "Core/Log.h"

#include <algorithm>

namespace Verlet
{
	namespace Physics
	{
		void phSystem::Update()
		{
			float deltaTime = settings.updateRate;
			assert(deltaTime > 0.0f);
			
			if (isSimulationPaused || deltaTime <= 0.0f)
				return;

			float scaledDeltaTime = deltaTime / settings.substeps;
			float damper = 1 - (settings.velocityDamper * scaledDeltaTime);

			glm::vec3 acceleration = VEC3_UP * -settings.gravity;

			for (uint32_t substep = 0; substep < settings.substeps; substep++)
			{
				// Update Objects
				for (uint32_t i = 0; i < m_objects.size(); i++)
				{
					phObject* object = m_objects[i];
					object->Update(acceleration, scaledDeltaTime, damper);
				}
			}
		}

#if DEBUG_FEATURES
		void phSystem::DrawDebug()
		{

			int updateFps = (int)ceil(1 / settings.updateRate);

			ImGui::SetNextWindowSize(ImVec2(500.0f, 500.0f), ImGuiCond_FirstUseEver);
			ImGui::Begin("Physics System");
			ImGui::SeparatorText("Simulation Settings");
			ImGui::SliderFloat("Gravity", &settings.gravity, -25, 25);
			ImGui::SliderFloat("Velocity Damper", &settings.velocityDamper, 0, 1, "%.4f");
			ImGui::SliderInt("Update Rate", &updateFps, 20, 240, "%d/fps");
			ImGui::SliderInt("Substeps", (int*)&(settings.substeps), 1, 50, "%d");
			ImGui::Checkbox("Pause Simulation", &isSimulationPaused);

			settings.updateRate = 1.0f / updateFps;

			ImGui::SeparatorText("Objects");
			ImGui::BeginTable("objects", 2, ImGuiTableFlags_Borders);
			ImGui::TableSetupColumn("Memory Address", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableHeadersRow();
			for (uint32_t i = 0; i < m_objects.size(); i++)
			{
				phObject* object = m_objects[i];
				object->DrawDebug();

				ImGui::TableNextColumn();
				ImGui::Text(object->GetMemoryAddress());
				ImGui::TableNextColumn();
				if (
					ImGui::Button("Edit") && 
					std::find(m_exploringObjects.begin(), m_exploringObjects.end(), object) == m_exploringObjects.end()
					)
				{
					m_exploringObjects.push_back(object);
				}
			}
			ImGui::EndTable();

			ImGui::End();

			for (uint32_t i = 0; i < m_exploringObjects.size(); i++)
			{
				phObject* object = m_exploringObjects[i];
				bool isOpen = true;
				object->DrawWidgets(&isOpen);

				if (!isOpen)
					m_exploringObjects.erase(m_exploringObjects.begin() + i);
			}
		}
#endif // DEBUG_FEATURES
	}
}
