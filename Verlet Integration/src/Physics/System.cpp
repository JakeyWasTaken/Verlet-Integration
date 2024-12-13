#include "System.h"
#include "Rendering/Debug/DebugImGui.h"
#include "Core/Random.h"
#include "Core/Vector.h"

namespace Verlet
{
	namespace Physics
	{
		void System::Update(float deltaTime)
		{
			if (isSimulationPaused)
				return;

			m_lastUpdate = Time::Time;

			float scaledDeltaTime = deltaTime / simulationSubsteps;

			for (uint32_t substep = 0; substep < simulationSubsteps; substep++)
			{
				// Initial update step
				for (uint32_t i = 0; i < m_points.size(); i++)
				{
					Point* point = m_points[i];
					point->Update(glm::vec3(0.0f, 1.0f, 0.0f) * -simulationGravity, scaledDeltaTime);
				}

				// Solve constraints
				for (uint32_t i = 0; i < m_constraints.size(); i++)
				{
					Constraint* constraint = m_constraints[i];
					constraint->Solve(scaledDeltaTime);
				}

				// Solve new point velocities
				for (uint32_t i = 0; i < m_points.size(); i++)
				{
					Point* point = m_points[i];
					//glm::vec3 travelDirection = SafeNormalize(point->GetPosition() - point->GetLastPosition());
					//point->SetVelocity(travelDirection * glm::dot(travelDirection, point->GetVelocity()));
					float gain = (1.0f - (0.005f * scaledDeltaTime));
					point->SetVelocity(((point->GetPosition() - point->GetLastPosition()) / scaledDeltaTime) * gain);
				
					//point->SetVelocity(point->GetPosition() - point->GetLastPosition());
				}
			}
		}

		void System::DrawWidgets()
		{
			static float compliance = 0.001f;
			bool doImpulse = false;

			ImGui::SetNextWindowSize(ImVec2(500.0f, 500.0f), ImGuiCond_FirstUseEver);
			ImGui::Begin("Physics System");
			ImGui::SliderFloat("Gravity", &simulationGravity, -25, 25);
			ImGui::SliderFloat("Compliance", &compliance, 0.00000001f, 0.1f, "%.10f");
			if (ImGui::Button("Apply Impulse"))
				doImpulse = true;

			ImGui::Checkbox("Pause", &isSimulationPaused);
			ImGui::End();

			for (uint32_t i = 0; i < m_points.size(); i++)
			{
				Point* point = m_points[i];
				point->DrawDebug();

				if (doImpulse)
				{
					float x = Random::RandomFloat();
					float y = Random::RandomFloat();
					float z = Random::RandomFloat();

					point->Impulse(glm::vec3(x, y, z));
				}
			}
		
			for (uint32_t i = 0; i < m_constraints.size(); i++)
			{
				Constraint* constraint = m_constraints[i];
				constraint->compliance = compliance;
			}
		}
	}
}
