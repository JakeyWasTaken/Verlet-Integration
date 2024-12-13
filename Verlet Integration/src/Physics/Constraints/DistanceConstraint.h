#pragma once
#include "Constraint.h"
#include "Physics/Point.h"
#include "glm/glm.hpp"

namespace Verlet
{
	namespace Physics
	{
		class DistanceConstraint : public Constraint
		{
		public:
			float restLength;

			DistanceConstraint(Point* point0, Point* point1, float compliance = 0.0f) :
				m_point0(point0),
				m_point1(point1),
				Constraint(compliance),
				restLength(0.0f) {
					restLength = glm::length(point0->GetPosition() - point1->GetPosition());
				}

			void Solve(float deltaTime);

			inline Point* GetPoint0() { return m_point0; };
			inline Point* GetPoint1() { return m_point1; };
		private:
			Point* m_point0 = nullptr;
			Point* m_point1 = nullptr;
		};
	}
}

