#pragma once

#include <glad/glad.h>

#include <stdexcept>
#include <vector>

struct BufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
};

class VertexBufferLayout
{
private:
	std::vector<BufferElement> m_Elements;
	unsigned int m_stride;
public:
	VertexBufferLayout() : m_stride{ 0 } {}
	~VertexBufferLayout() {}

	template<typename T> // partially specified template
	void push(unsigned int count)
	{
		std::runtime_error(false);
	}
	
	template<>
	void push<float>(unsigned int count) // fully specialised template
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride += sizeof(GL_FLOAT) * count;
	}
	
	template<>
	void push<unsigned int>(unsigned int count) // fully specialised template
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += sizeof(GL_UNSIGNED_INT) * count;
	}

	template<typename T>
	void push_empty(unsigned int count)
	{
		std::runtime_error(false);
	}

	template<>
	void push_empty<float>(unsigned int count)
	{
		m_stride += sizeof(GL_FLOAT) * count;
	}

	template<>
	void push_empty<unsigned int>(unsigned int count)
	{
		m_stride += sizeof(GL_UNSIGNED_INT) * count;
	}

	inline const std::vector<BufferElement>& elements() const
	{
		return m_Elements;
	}

	inline unsigned int stride() const
	{
		return m_stride;
	}
};
