#pragma once
#include "Constraint.h"

namespace Verlet
{
	namespace Physics
	{
		class DistanceConstraint : public Constraint
		{
		public:
			float restLength;

			DistanceConstraint(phPoint* point0, phPoint* point1, float compliance = 0.0f) :
				m_point0(point0),
				m_point1(point1),
				Constraint(compliance),
				restLength(0.0f) {
					restLength = glm::length(point0->GetPosition() - point1->GetPosition());
				}

			void Solve(float deltaTime);

			inline phPoint* GetPoint0() { return m_point0; };
			inline phPoint* GetPoint1() { return m_point1; };
		private:
			phPoint* m_point0 = nullptr;
			phPoint* m_point1 = nullptr;
		};
	}
}

