#pragma once

#include <stdint.h>
#include <glad/glad.h>

/**
* Use this over a render buffer if you need to sample the depth or stencil values
*
* Otherwise if you don't intend to sample them, use a render buffer
*/
class DepthStencilTextureAttachment
{
private:
	uint32_t m_DST_id;

public:
	DepthStencilTextureAttachment(uint32_t width, uint32_t height)
	{
		glGenTextures(1, &m_DST_id);
		bind();

		// allocates memory but does not actually fill it
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		unbind();
	}

	~DepthStencilTextureAttachment()
	{
		glDeleteTextures(1, &m_DST_id);
	}

	uint32_t get_id() const
	{
		return m_DST_id;
	}

	void bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_DST_id);
	}

	void unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};
