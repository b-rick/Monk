#pragma once

#include <glm/glm.hpp>

namespace Light
{

	struct ABSColors
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	struct DirectionLight
	{
		glm::vec3 direction;

		ABSColors colors;
	};

	struct PointLight
	{
		glm::vec3 position;
		
		// attenuation
		float attn_const;
		float attn_linear;
		float attn_quad;

		ABSColors colors;
	};

	struct SpotLight
	{
		glm::vec3 position;
		glm::vec3 direction;
		
		// cut off values
		float cutoff;
		float outerCutoff;
		
		// attenuation values
		float attn_const;
		float attn_linear;
		float attn_quad;
		
		ABSColors colors;
	};
}

