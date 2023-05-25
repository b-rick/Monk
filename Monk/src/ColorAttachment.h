#pragma once

#include <stdint.h>
#include <glad/glad.h>

class ColorTextureAttachment
{
private:
	uint32_t m_CT_id;

public:
	ColorTextureAttachment(uint32_t width, uint32_t height)
	{
		glGenTextures(1, &m_CT_id);
		bind();

		// allocates memory but does not actually fill it
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unbind();
	}

	~ColorTextureAttachment()
	{
		glDeleteTextures(1, &m_CT_id);
	}

	uint32_t get_id() const
	{
		return m_CT_id;
	}

	void bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_CT_id);
	}

	void unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};
