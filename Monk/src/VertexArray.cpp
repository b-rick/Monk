#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::add_element_buffer(VertexBuffer* vbo, ElementBuffer* ebo, const VertexBufferLayout& layout) const
{
	bind();
	vbo->bind();
	ebo->bind();
	const auto& elements = layout.elements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
#pragma warning(suppress:4312) // casting from unsigned int to const void * because open gl has a silly api
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.stride(), reinterpret_cast<GLvoid*>(offset));
		glEnableVertexAttribArray(i);
		offset += element.count * sizeof(element.type);
	}
}

void VertexArray::addBuffer(VertexBuffer* vbo, const VertexBufferLayout& layout) const
{
	bind();
	vbo->bind();
	const auto& elements = layout.elements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		#pragma warning(suppress:4312) // casting from unsigned int to const void * because open gl has a silly api
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.stride(), reinterpret_cast<GLvoid*>(offset));
		glEnableVertexAttribArray(i);
		offset += element.count * sizeof(element.type);
	}
}

void VertexArray::bind() const
{
	glBindVertexArray(m_ID);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}
