#pragma once

#include "ElementBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_ID;
public:
	VertexArray();
	~VertexArray();
	
	void add_element_buffer(VertexBuffer* vbo, ElementBuffer* ebo, const VertexBufferLayout& layout) const;
	void addBuffer(VertexBuffer* vbo, const VertexBufferLayout& layout) const;
	void bind() const;
	void unbind() const;
};
