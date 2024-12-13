#include "DistanceConstraint.h"

namespace Verlet
{
	namespace Physics
	{
		void DistanceConstraint::Solve(float deltaTime)
		{
			float invMass0 = m_point0->GetInvMass();
			float invMass1 = m_point1->GetInvMass();

			float sumMass = invMass0 + invMass1;
			if (sumMass == 0.0f)
				return;

			glm::vec3 pointDifference = m_point1->GetPosition() - m_point0->GetPosition();
			glm::vec3 direction = glm::normalize(m_point1->GetPosition() - m_point0->GetPosition());
			float distance = glm::length(pointDifference);
			float constraint = distance - restLength;

			/*if (constraint == 0.0f)
				return;*/

			float scaledCompliance = compliance / (deltaTime * deltaTime);

			float dLambda = (-constraint - scaledCompliance) / (sumMass + scaledCompliance);

			glm::vec3 gradientVector = constraint * direction;

			m_point0->AddPosition(+invMass0 / sumMass * gradientVector);
			m_point1->AddPosition(-invMass1 / sumMass * gradientVector);
		};
	}
}