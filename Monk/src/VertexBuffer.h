#pragma once
#include <glad/glad.h>

#include <iostream>
#include <vector>
#include <vector>

class VertexBuffer
{
private:
	unsigned int m_ID;
public:
	VertexBuffer(const void* vertices, unsigned int size, GLenum drawType)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, drawType);
	}

	void bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	~VertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}
};
