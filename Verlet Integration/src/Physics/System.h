/*
* // THEORY \\
* 
* The Physics system is entirely for simulating point masses constrainted to one another that can collide
* IT DOES NOT interact with any meshes / models etc.
* 
* This system aims to implement XPBD in a robust and extendable way
*/

#pragma once
#include "Point.h"
#include "Constraints/Constraint.h"
#include "glm/glm.hpp"
#include "stdint.h"
#include "Core/Time.h"
#include <vector>

namespace Verlet
{
	namespace Physics
	{
		class System
		{
		public:
			uint32_t simulationSubsteps = 20;
			float simulationGravity = 9.8;
			float updateRate = 1.0f / 120.0f;
			bool isSimulationPaused = false;
			
			void Update(float deltaTime);
			void DrawWidgets();

			inline bool ReadyToUpdate() { return Time::Time - m_lastUpdate >= updateRate; };

			inline void AddPoint(Point* point) { m_points.push_back(point); };
			inline void AddConstraint(Constraint* constraint) { m_constraints.push_back(constraint); };

		private:
			std::vector<Point*> m_points;
			std::vector<Constraint*> m_constraints;

			float m_lastUpdate = 0.0f;
		};
	}
}

