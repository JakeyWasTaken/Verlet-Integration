#pragma once
#include "glm/glm.hpp"
#include "Core/Input.h"
#include "Config.h"
#include "Core/Vector.h"

namespace Verlet
{
	class Camera
	{
	public:
		Camera(glm::vec3 position);

		inline bool IsMouseLocked() { return m_mouseLocked; };
		inline glm::vec3 GetPosition() { return m_position; };
		inline glm::vec3 GetDirection() { return m_direction; };

		void SetMouseLocked(bool locked);
		void SolveCameraDirection();

		void ProcessKeyboard();
		void ProcessMouse();

		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();
	private:
		float m_speed = CONF_CAMERA_SPEED;

		float m_yaw = 0.0f;
		float m_pitch = 0.0f;

		float m_FOV = CONF_CAMERA_FIELD_OF_VIEW;

		double m_mouseX = 0;
		double m_mouseY = 0;

		bool m_mouseLocked = false;

		uint32_t m_lastViewMatrixFrame = -1;
		uint32_t m_lastProjMatrixFrame = -1;

		glm::mat4 m_bufferedViewMatrix;
		glm::mat4 m_bufferedProjMatrix;

		glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 10.0f);
		glm::vec3 m_direction = glm::normalize(m_position - glm::vec3(0.0f, 0.0f, 0.0f));
		glm::vec3 m_right = glm::normalize(glm::cross(VEC3_UP, m_direction));
		glm::vec3 m_up = glm::cross(m_direction, m_right);
	};
}