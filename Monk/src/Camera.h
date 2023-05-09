#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float MOUSE_SENSITIVITY = 0.1f;
const float MOVEMENT_SENSITIVITY = 1.0f;
const float ZOOM_SENSITIVITY = 2.0f;
const float FOV = 45.0f;

const glm::vec3 ZERO = glm::vec3(0, 0, 0);
const glm::vec3 UNIT_Y = glm::vec3(0, 1, 0);

const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 100.0f;

enum CameraMovement
{
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT,
	UP,
	DOWN
};

class Camera
{
private:
	// probably won't be changed much
	float m_MouseSensitivity;
	float m_MovementSensitivity;
	float m_ZoomSensitivity;

	float m_FOV;
	unsigned int m_screenWidth;
	unsigned int m_screenHeight;
		
	// changed basically every frame
	float m_yaw;
	float m_pitch;
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_right;
	glm::vec3 m_up;

	// projection matrix values
	float m_near_plane;
	float m_far_plane;

	inline void update_camera_vectors()
	{
		auto dirX = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
		auto dirY = glm::sin(glm::radians(m_pitch));
		auto dirZ = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
		auto direction = glm::vec3{ dirX, dirY, dirZ };

		m_front = glm::normalize(direction);
		m_right = glm::normalize(glm::cross(m_front, UNIT_Y));
		m_up = glm::normalize(glm::cross(m_right, m_front));
	}

public:
	Camera(unsigned int screenWidth, unsigned int screenHeight) :
		m_MouseSensitivity{ MOUSE_SENSITIVITY },
		m_MovementSensitivity{ MOVEMENT_SENSITIVITY },
		m_ZoomSensitivity{ ZOOM_SENSITIVITY },
		m_FOV{ FOV },
		m_screenWidth{ screenWidth },
		m_screenHeight{ screenHeight },
		m_yaw{ YAW },
		m_pitch{ PITCH },
		m_position{ glm::vec3(0.0f, 0.0f, -3.0f)},
		m_up { UNIT_Y },
		m_near_plane { NEAR_PLANE },
		m_far_plane { FAR_PLANE }
	{
		update_camera_vectors();
	}

	~Camera() {}

	void process_keypress(CameraMovement direction, float dt)
	{
		auto velocity = m_MovementSensitivity * dt;

		if (direction == FORWARD)
		{
			m_position += m_front * velocity;
		}
		else if (direction == BACKWARD)
		{
			m_position -= m_front * velocity;
		}
		else if (direction == RIGHT)
		{
			m_position += m_right * velocity;
		}
		else if (direction == LEFT)
		{
			m_position -= m_right * velocity;
		}
		else if (direction == UP)
		{
			m_position += m_up * velocity;
		}
		else if (direction == DOWN)
		{
			m_position -= m_up * velocity;
		}
	}

	void process_mouse_movement(float dx, float dy)
	{
		float xOffset = dx * m_MouseSensitivity;
		float yOffset = dy * m_MouseSensitivity;

		m_yaw += xOffset;
		m_pitch += yOffset;
		
		m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
		update_camera_vectors();
	}

	void process_mouse_scroll(float yoff)
	{
		float zoom = yoff * m_ZoomSensitivity;
		m_FOV -= zoom;
		m_FOV = glm::clamp(m_FOV, 1.0f, 120.0f);
	}

	void process_resize(unsigned int width, unsigned int height)
	{
		m_screenWidth = width;
		m_screenHeight = height;
	}

	inline const glm::mat4 view_matrix() const
	{
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}

	inline const glm::mat4 projection_matrix() const
	{
		return glm::perspective(glm::radians(m_FOV), (float) m_screenWidth / (float) m_screenHeight, m_near_plane, m_far_plane);
	}

	inline const glm::vec3 front() const
	{
		return m_front;
	}

	inline const glm::vec3 position() const
	{
		return m_position;
	}

	inline void set_near_plane(float a_near_plane)
	{
		m_near_plane = a_near_plane;
	}

	inline void set_far_plane(float a_far_plane)
	{
		m_far_plane = a_far_plane;
	}
};
