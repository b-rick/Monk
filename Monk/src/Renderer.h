#pragma once

#include "Texture.h"

#include <string>
#include <unordered_map>

class Renderer
{
private:	
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
public:
	Renderer()
	{
		this->m_textures = std::unordered_map<std::string, std::unique_ptr<Texture>>{};
	}

	Texture* load_texture(const std::string& a_path, TextureType a_type)
	{
		auto m_texture_itr = m_textures.find(a_path);
		if (m_texture_itr != m_textures.end())
		{
			return m_texture_itr->second.get();
		}
		auto result = m_textures.emplace(a_path, std::make_unique<Texture>(a_path, a_type));
		return result.first->second.get();
	}
};
