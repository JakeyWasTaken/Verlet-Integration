#include "Camera.h"
#include "Core/Window.h"
#include "Core/Input.h"
#include "Core/Time.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Verlet
{
	Camera::Camera(glm::vec3 position)
	{
		m_position = position;

		// Solve matrix for vectors on creation
		SetMouseLocked(false);
		SolveCameraDirection();
		GetViewMatrix();
	}
	
	void Camera::SetMouseLocked(bool locked)
	{
		m_mouseLocked = locked;

		GLFWwindow* window = CurrentWindow->GetGLFWWindow();

		if (m_mouseLocked)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Camera::SolveCameraDirection()
	{
		glm::vec3 direction;
		direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		direction.y = sin(glm::radians(m_pitch));
		direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_direction = glm::normalize(direction);
	}

	void Camera::ProcessKeyboard()
	{
		float scaledSpeed = m_speed * Time::DeltaTime * (GET_KEY_STATE(GLFW_KEY_LEFT_SHIFT) == KEY_PRESS ? 10 : 1);

		if (GET_KEY_STATE(GLFW_KEY_W) == KEY_PRESS)
			m_position += m_direction * scaledSpeed;
		if (GET_KEY_STATE(GLFW_KEY_S) == KEY_PRESS)
			m_position += -m_direction * scaledSpeed;
		if (GET_KEY_STATE(GLFW_KEY_A) == KEY_PRESS)
			m_position += m_right * scaledSpeed;
		if (GET_KEY_STATE(GLFW_KEY_D) == KEY_PRESS)
			m_position += -m_right * scaledSpeed;
		if (GET_KEY_STATE(GLFW_KEY_E) == KEY_PRESS)
			m_position += m_up * scaledSpeed;
		if (GET_KEY_STATE(GLFW_KEY_Q) == KEY_PRESS)
			m_position += -m_up * scaledSpeed;

		if (GET_MOUSE_BUTTON_STATE(RIGHT_MOUSE_BUTTON) == MOUSE_PRESS)
			SetMouseLocked(true);
		else
			SetMouseLocked(false);
	}

	void Camera::ProcessMouse()
	{
		double bufferedMouseX = 0;
		double bufferedMouseY = 0;

		GET_MOUSE_POSITION(&bufferedMouseX, &bufferedMouseY);

		double deltaX = bufferedMouseX - m_mouseX;
		double deltaY = bufferedMouseY - m_mouseY;

		m_mouseX = bufferedMouseX;
		m_mouseY = bufferedMouseY;

		if (!m_mouseLocked)
			return;

		m_yaw += deltaX * CONF_CAMERA_SENS;
		m_pitch += -deltaY * CONF_CAMERA_SENS; // Flip vertical mouse movement

		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;

		SolveCameraDirection();
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		if (m_lastViewMatrixFrame == Time::Frame)
			return m_bufferedViewMatrix;

		m_right = glm::normalize(glm::cross(VEC3_UP, m_direction));
		m_up = glm::cross(m_direction, m_right);
		m_bufferedViewMatrix = glm::lookAt(m_position, m_position + m_direction, m_up);
		m_lastViewMatrixFrame = Time::Frame;

		return m_bufferedViewMatrix;
	}

	glm::mat4 Camera::GetProjectionMatrix()
	{
		if (m_lastProjMatrixFrame == Time::Frame)
			return m_bufferedProjMatrix;

		Window* window = CurrentWindow;

		float aspect = (float)window->GetWidth() / (float)window->GetHeight();

		if (aspect != aspect) // prevent assertion when minimized (checks if NaN)
			return glm::mat4(1.0f);

		m_bufferedProjMatrix = glm::perspective(glm::radians(m_FOV), aspect, CONF_CAMERA_NEAR_PLANE, CONF_CAMERA_FAR_PLANE);
		m_lastProjMatrixFrame = Time::Frame;

		return m_bufferedProjMatrix;
	}
}
