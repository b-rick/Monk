#pragma once

#include <glad/glad.h>
#include <iostream>
#include <sbti/stb_image.h>

enum TextureType { SPECULAR, DIFFUSE };

class Texture
{
private:
	TextureType m_texture_type;
	unsigned int m_ID;
	std::string m_path;
public:
	Texture(const std::string& a_path, TextureType a_type)
		: m_path{ a_path },
		m_texture_type{ a_type }
	{
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		auto width = int{};
		auto height = int{};
		auto num_channels = int{};
		unsigned char* data = stbi_load(a_path.c_str(), &width, &height, &num_channels, 0);
		if (data)
		{
			GLenum format;
			if (num_channels == 1)
			{
				format = GL_RED;
			}
			else if (num_channels == 3)
			{
				format = GL_RGB;
			}
			else if (num_channels == 4)
			{
				format = GL_RGBA;
			}
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);
		}
		else
		{
			std::cerr << "Failed to load image texture" << std::endl;
		}
	}

	~Texture()
	{
		glDeleteTextures(1, &m_ID);
	}

	inline void use(GLenum textureUnit)
	{
		glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	inline TextureType getType() const
	{
		return m_texture_type;
	}

	inline const std::string& getPath() const
	{
		return m_path;
	}
};
