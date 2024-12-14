#pragma once
#include "glm/vec3.hpp"
#include "Core/Vector.h"

namespace Verlet
{
	namespace Physics
	{
		class phPoint
		{
		public:
			phPoint(glm::vec3 position, float mass = 0.0f) :
				m_position(position),
				m_lastPosition(position),
				m_velocity(VEC3_ZERO) {
					SetMass(mass);
				}

			void Update(glm::vec3 acceleration, float deltaTime);
			void Impulse(glm::vec3 velocity);
			void AddPosition(glm::vec3 position);
			void DrawDebug();

			// Also updates invMass
			inline void SetMass(float mass) { m_mass = mass; m_invMass = mass > 0 ? 1 / mass : 0; };
			inline float GetMass() const { return m_mass; };
			inline float GetInvMass() const { return m_invMass; };

			inline glm::vec3 GetPosition() const { return m_position; };
			inline glm::vec3 GetLastPosition() const { return m_lastPosition; };

			inline glm::vec3 GetVelocity() const { return m_velocity; };
			inline void SetVelocity(glm::vec3 velocity) { m_velocity = velocity; };

		private:
			float m_mass = 0;
			float m_invMass = 0;

			glm::vec3 m_position;
			glm::vec3 m_lastPosition;
			glm::vec3 m_velocity;
		};
	}
}

