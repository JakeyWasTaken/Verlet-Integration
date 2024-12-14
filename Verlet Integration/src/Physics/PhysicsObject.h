#pragma once
#include "PhysicsPoint.h"
#include "Constraints/Constraint.h"
#include "Config.h"
#include <vector>

namespace Verlet
{
	namespace Physics
	{
		class phObject
		{
		public:
			void Update(glm::vec3 acceleration, float deltaTime, float velocityDamper);

#if DEBUG_FEATURES
			// ImGui widgets, returns should close
			void DrawWidgets(bool* windowOpen);

			// Debug Draw
			void DrawDebug();

			const char* GetMemoryAddress();
#endif

			inline void AddPoint(phPoint* point) { m_points.push_back(point); };
			inline void AddConstraint(Constraint* constraint) { m_constraints.push_back(constraint); };
		private:
			std::vector<phPoint*> m_points;
			std::vector<Constraint*> m_constraints;
		};
	}
}
