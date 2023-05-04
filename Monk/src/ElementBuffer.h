#pragma once

#include <stdint.h>
#include <glad/glad.h>

class ElementBuffer
{
private:
	uint32_t m_EBO;
public:
	ElementBuffer(const void* indices, uint32_t size, GLenum drawType) 
	{
		glGenBuffers(1, &m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, drawType);
	}

	~ElementBuffer() 
	{
		glDeleteBuffers(1, &m_EBO);
	}

	void bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	}

	void unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
};
