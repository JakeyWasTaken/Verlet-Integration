#pragma once
// all includes here get included in every other constraint aswell
#include "Core/Vector.h"
#include "Physics/Point.h"
#include "glm/glm.hpp"

namespace Verlet
{
	namespace Physics
	{
		class Constraint
		{
		public:
			float compliance = 0.0f;

			Constraint(float compliance = 0.0f) :
				compliance(compliance) {}

			virtual void Solve(float deltaTime) {};
		};
	}
}

