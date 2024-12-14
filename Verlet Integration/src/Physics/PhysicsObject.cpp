#include "PhysicsObject.h"
#include "Core/Log.h"
#include "Core/Random.h"

#include "Rendering/Debug/DebugImGui.h"

#pragma warning(disable : 4996)

namespace Verlet
{
	namespace Physics
	{
		void phObject::Update(glm::vec3 acceleration, float deltaTime, float velocityDamper)
		{
			// Initial update step
			for (uint32_t i = 0; i < m_points.size(); i++)
			{
				phPoint* point = m_points[i];
				point->Update(acceleration, deltaTime);
			}

			// Solve constraints
			for (uint32_t i = 0; i < m_constraints.size(); i++)
			{
				Constraint* constraint = m_constraints[i];
				constraint->Solve(deltaTime);
			}

			// Solve new point velocities
			for (uint32_t i = 0; i < m_points.size(); i++)
			{
				phPoint* point = m_points[i];
				point->SetVelocity(
					(point->GetPosition() - point->GetLastPosition()) / deltaTime 
					* velocityDamper
				);
			}
		}

#if DEBUG_FEATURES
		void phObject::DrawWidgets(bool* windowOpen)
		{
			const char* objectAddress = GetMemoryAddress();

			char title[128];
			sprintf(title, "Object (%s)", objectAddress);

			bool applyImpulse;
			static float impulseScale = 1;

			ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Appearing);
			ImGui::Begin(title, windowOpen);

			ImGui::SliderFloat("Impulse Scale", &impulseScale, 0, 25);
			applyImpulse = ImGui::Button("Apply Impulse");

			ImGui::End();

			for (uint32_t i = 0; i < m_points.size(); i++)
			{
				phPoint* point = m_points[i];
				
				if (applyImpulse)
				{
					float x = Random::RandomFloat(-1.0f);
					float y = Random::RandomFloat(-1.0f);
					float z = Random::RandomFloat(-1.0f);

					point->Impulse(glm::vec3(x, y, z) * impulseScale);
				}
			}
		}

		void phObject::DrawDebug()
		{
			for (uint32_t i = 0; i < m_points.size(); i++)
			{
				phPoint* point = m_points[i];
				point->DrawDebug();
			}
		}

		const char* phObject::GetMemoryAddress()
		{
			char objectAddress[] = "0x00000000";
			sprintf(objectAddress, "0x%x", (int)this);

			return objectAddress;
		}
#endif // DEBUG_FEATURES
	}
}