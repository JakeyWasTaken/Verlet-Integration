#pragma once

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

