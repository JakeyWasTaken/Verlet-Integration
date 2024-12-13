#include "Point.h"
#include "Core/Log.h"
#include "Rendering/Debug/DebugDraw.h"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Verlet
{
	namespace Physics
	{
		Point::Point(glm::vec3 position, float mass)
		{
			m_position = position;
			m_lastPosition = position;
			m_velocity = glm::vec3(0.0f);

			SetMass(mass);
		}

		void Point::Update(glm::vec3 acceleration, float deltaTime)
		{
			if (m_invMass <= 0.0f)
				return;

			m_velocity += deltaTime * acceleration;
			m_lastPosition = m_position;
			m_position += m_velocity * deltaTime;

			if (m_position.y < 0.0f)
			{
				m_position.y = 0.0f;
				m_velocity = -m_velocity;
				m_velocity *= 0.75f;
			}
		}

		void Point::Impulse(glm::vec3 velocity)
		{
			m_velocity += velocity;
		}

		void Point::AddPosition(glm::vec3 position)
		{
			m_position += position;
		}

		void Point::DrawDebug()
		{
			grDebugDraw::Sphere(m_position, 0.1f, 2);
			//grDebugDraw::Arrow(m_position, m_position + glm::normalize(m_velocity), 0.075f, 0.25f, 20, COLOR_RGB(242, 78, 7));
		}
	}
}