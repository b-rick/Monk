#pragma once

#include <stdint.h>
#include <glad/glad.h>

/**
* Render buffers aren't really used for reading, but they are useful for stencil and depth buffers
* We need the depth and stencil buffers for testing, but we don't need to sample the actual values.
*
* When we aren't sampling from a buffer, a renderbuffer object is preferred.
*/
class RenderBuffer
{
private:
	uint32_t m_id;
	GLenum m_type;

public:
	RenderBuffer(GLenum type, uint32_t width, uint32_t height) : m_type{type}
	{
		glGenRenderbuffers(1, &m_id);
		bind();
		glRenderbufferStorage(GL_RENDERBUFFER, type, width, height);
		unbind();
	}

	~RenderBuffer()
	{
		glDeleteTextures(1, &m_id);
	}

	uint32_t get_id() const
	{
		return m_id;
	}

	void bind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_id);
	}

	void unbind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
};
