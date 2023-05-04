#pragma once

#include<glm/glm.hpp>
#include<array>
#include<imgui.h>

class LightingWindow
{
private:
	bool window_open;
	bool close_app;

	// direction light values
	float m_direction_light_direction[3];
	float m_direction_light_ambient[3];
	float m_direction_light_diffuse[3];
	float m_direction_light_specular[3];

	// Point light values
	float m_point_light_ambient[3];
	float m_point_light_diffuse[3];
	float m_point_light_specular[3];
	
	float m_point_attn_const;
	float m_point_attn_linear;
	float m_point_attn_quad;

	
	// Spot Light values
	float m_spot_light_ambient[3];
	float m_spot_light_diffuse[3];
	float m_spot_light_specular[3];

	float m_spot_attn_const;
	float m_spot_attn_linear;
	float m_spot_attn_quad;
	
	float m_cutoff_angle;
	float m_outerCutoff_angle;

public:
	LightingWindow() :
		window_open{ false },
		close_app{ false },

		// direction light init
		m_direction_light_direction{ 0.2f, -1.0f, -0.3f },

		m_direction_light_ambient{ 0.05f, 0.05f, 0.05f },
		m_direction_light_diffuse{ 0.4f, 0.4f, 0.4f },
		m_direction_light_specular{ 0.5f, 0.5f, 0.5f },

		// point light init
		m_point_light_ambient{ 0.05f, 0.05f, 0.05f },
		m_point_light_diffuse{ 0.4f, 0.4f, 0.4f },
		m_point_light_specular{ 0.5f, 0.5f, 0.5f },

		m_point_attn_const{ 1.0f },
		m_point_attn_linear{ 0.09f },
		m_point_attn_quad{ 0.032f },

		// Flashlight init
		m_spot_light_ambient{ 0.0f, 0.0f, 0.0f },
		m_spot_light_diffuse{ 1.0f, 1.0f, 1.0f },
		m_spot_light_specular{ 1.0f, 1.0f, 1.0f },

		m_spot_attn_const{ 1.0f },
		m_spot_attn_linear{ 0.09f },
		m_spot_attn_quad{ 0.032f },

		m_cutoff_angle{ glm::radians(12.5f) },
		m_outerCutoff_angle{ glm::radians(15.0f) }
	{}


	void draw_window()
	{
		if (window_open) 
		{
			ImGui::Begin("Lighting controller", &window_open);
			ImGui::Text("Directional light");
			ImGui::SliderFloat("Direction X: ", &m_direction_light_direction[0], -3.0f, 3.0f);
			ImGui::SliderFloat("Direction Y: ", &m_direction_light_direction[1], -3.0f, 3.0f);
			ImGui::SliderFloat("Direction Z: ", &m_direction_light_direction[2], -3.0f, 3.0f);

			ImGui::ColorEdit3("Direction light ambient: ", m_direction_light_ambient);
			ImGui::ColorEdit3("Direction light diffuse: ", m_direction_light_diffuse);
			ImGui::ColorEdit3("Direction light specular: ", m_direction_light_specular);

			ImGui::Text("Point lights");
			ImGui::ColorEdit3("Point light ambient: ", m_point_light_ambient);
			ImGui::ColorEdit3("Point light diffuse: ", m_point_light_diffuse);
			ImGui::ColorEdit3("Point light specular: ", m_point_light_specular);

			ImGui::Text("Point lights attn");
			ImGui::SliderFloat("Point attn const: ", &m_point_attn_const, 0.0f, 1.0f);
			ImGui::SliderFloat("Point attn linear: ", &m_point_attn_linear, 0.0f, 1.0f);
			ImGui::SliderFloat("Point attn quad: ", &m_point_attn_quad, 0.0f, 1.0f);


			ImGui::Text("Flash light");
			ImGui::ColorEdit3("Flash light ambient: ", m_spot_light_ambient);
			ImGui::ColorEdit3("Flash light diffuse: ", m_spot_light_diffuse);
			ImGui::ColorEdit3("Flash light specular: ", m_spot_light_specular);
		
			ImGui::Text("Flash light attn");
			ImGui::SliderFloat("const: ", &m_spot_attn_const, 0.0f, 1.0f);
			ImGui::SliderFloat("linear: ", &m_spot_attn_linear, 0.0f, 1.0f);
			ImGui::SliderFloat("quad: ", &m_spot_attn_quad, 0.0f, 1.0f);
			
			ImGui::Text("Flash light cutoff angle");
			ImGui::SliderAngle("inner", &m_cutoff_angle, 0.0f, 45.0f);
			ImGui::SliderAngle("cutoff", &m_outerCutoff_angle, 0.0f, 45.0f);


			ImGui::SameLine();
			if (ImGui::Button("Close App"))
				close_app = true;
			ImGui::End();
		}
	}

	inline bool is_window_open() const
	{
		return window_open;
	}

	void set_window_open(bool open)
	{
		window_open = open;
	}

	inline bool should_close_app() const
	{
		return close_app;
	}

	inline const glm::vec3 direction_light_direction() const
	{
		return make_vec3(m_direction_light_direction);
	}

	inline const glm::vec3 direction_light_ambient() const
	{
		return make_vec3(m_direction_light_ambient);
	}

	inline const glm::vec3 direction_light_diffuse() const
	{
		return make_vec3(m_direction_light_diffuse);
	}

	inline const glm::vec3 direction_light_specular() const
	{
		return make_vec3(m_direction_light_specular);
	}

	inline const glm::vec3 point_light_ambient() const
	{
		return make_vec3(m_point_light_ambient);
	}

	inline const glm::vec3 point_light_diffuse() const
	{
		return make_vec3(m_point_light_diffuse);
	}

	inline const glm::vec3 point_light_specular() const
	{
		return make_vec3(m_point_light_specular);
	}

	inline const float point_light_attn_const() const
	{
		return m_point_attn_const;
	}

	inline const float point_light_attn_linear() const
	{
		return m_point_attn_linear;
	}

	inline const float point_light_attn_quad() const
	{
		return m_point_attn_quad;
	}

	inline const glm::vec3 flash_light_ambient() const
	{
		return make_vec3(m_spot_light_ambient);
	}

	inline const glm::vec3 flash_light_diffuse() const
	{
		return make_vec3(m_spot_light_diffuse);
	}

	inline const glm::vec3 flash_light_specular() const
	{
		return make_vec3(m_spot_light_specular);
	}

	inline const float flash_light_attn_const() const
	{
		return m_spot_attn_const;
	}

	inline const float flash_light_attn_linear() const
	{
		return m_spot_attn_linear;
	}

	inline const float flash_light_attn_quad() const
	{
		return m_spot_attn_quad;
	}

	inline const float flash_light_cutoff() const
	{
		return glm::cos(m_cutoff_angle);
	}

	inline const float flash_light_outer_cutoff() const
	{
		return glm::cos(m_outerCutoff_angle);
	}

	inline static const glm::vec3 make_vec3(const float data[3])
	{
		return glm::vec3(data[0], data[1], data[2]);
	}
};
