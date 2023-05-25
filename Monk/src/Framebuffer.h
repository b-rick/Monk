#pragma once

#include <stdint.h>
#include <glad/glad.h>
#include <vector>

#include "ColorAttachment.h"
#include "DepthStencilAttachment.h"
#include "RenderBuffer.h"

class FrameBuffer
{
private:
	uint32_t m_FBO;
	std::vector<uint32_t> m_FB_color_textures;
	std::vector<uint32_t> m_FB_depth_stencil_textures;
	std::vector<uint32_t> m_FB_render_buffers;

public:
	FrameBuffer()
	{
		glGenFramebuffers(1, &m_FBO);
		bind();
	}

	~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_FBO);
	}

	/* binding to the framebuffer(fb) means all read and write operations affect this bound fb*/
	void bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	}

	static void bind_default()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void unbind()
	{
		bind_default();
	}

	void bind_read()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);
	}

	void bind_write()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
	}
	
	/** Remember to call glViewport before rendering to a fb */
	void attach_color_texture(const ColorTextureAttachment& texture)
	{
		auto id = texture.get_id();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);
		m_FB_color_textures.push_back(id);
	}

	void attach_depth_stencil_texture(const DepthStencilTextureAttachment& texture)
	{
		auto id = texture.get_id();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, id, 0);
		m_FB_depth_stencil_textures.push_back(id);
	}
	
	/** Renderbuffer: is a buffer like any other but cannot be directly read from. 
	* Store all the data directly in the buffer without any conversion to texture-specific formats
	* Faster as writeable storage medium
	* Cannot read from them directly, but can be read using the slow `glReadPixels` call.
	* Returns a specified area of pixels from the currently bound fb but not directly from the attachment itself.
	*/
	void attach_render_buffer(const RenderBuffer& rb)
	{
		auto id = rb.get_id();
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id);
		m_FB_render_buffers.push_back(id);
	}

	/*
	Each of the following could return:
	- GL_FRAMEBUFFER_COMPLETE -> good to go
	- GL_FRAMEBUFFER_UNDEFINED -> specified fb is read or write fb, but default does not exist
	- GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT -> if any attachment points are incomplete
	- GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT -> if fb does not have one image attached
	- GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER -> if value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 
			is GL_NONE for any color attachment oints named by GL_DRAW_BUFFERi
	- GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER -> if GL_READ_BUFFER is not GL_NONE and value 
			of GL_FB_ATTACHNENT_OBJECT_TYPE is GL_NONE for any color attachment points 
			named by GL_READ_BUFFER
	- GL_FRAMBUFFER_UNSUPPORTED if combination of intenral formats violates implementation
			dependent restrictuions
	- GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE -> if GL_RENDERBUFFER_SAMPLES is not same for all
			attached renderbuffers or if  GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not same for all 
			textures or if GL_TEXTURE_SAMPLES is not same for all attached textures or
			if attached images are a mix of renderbuffesr and textures, or if the value of
			GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures
	- GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS -> if fb attachment is layered and any
			populated attachemnt is not layered or if all color attachemnts are not from
			textures of the same target
	- 0 -> if any error occurs
	*/
	
	/* Check status of _this_ fb */
	GLenum check_named_status() const
	{
		return glCheckNamedFramebufferStatus(GL_FRAMEBUFFER, m_FBO);
	}
	
	/* Check status of currently bound fb */
	static GLenum check_status()
	{
		return glCheckFramebufferStatus(GL_FRAMEBUFFER);
	}

	static GLenum check_default_status()
	{
		return glCheckNamedFramebufferStatus(GL_FRAMEBUFFER, 0);
	}
};
