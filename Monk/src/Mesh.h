#pragma once

#include "ElementBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <glm/glm.hpp>

#include <memory>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class Mesh
{
private:
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<ElementBuffer> m_EBO;
	std::unique_ptr<VertexBuffer> m_VBO;

	std::vector<Vertex> m_vertices;
	std::vector<Texture*> m_textures;
	std::vector<uint32_t> m_indices;

	void setupMesh()
	{
		m_VBO = std::make_unique<VertexBuffer>(&m_vertices[0], static_cast<uint32_t>(m_vertices.size() * sizeof(Vertex)), GL_STATIC_DRAW);
		m_EBO = std::make_unique<ElementBuffer>(&m_indices[0], static_cast<uint32_t>(m_indices.size() * sizeof(uint32_t)), GL_STATIC_DRAW);

		auto vbl = VertexBufferLayout();
		vbl.push<float>(3); // vertices
		vbl.push<float>(3); // normal
		vbl.push<float>(2); // tex coords

		m_VAO = std::make_unique<VertexArray>();
		m_VAO->add_element_buffer(m_VBO.get(), m_EBO.get(), vbl);
		m_VAO->unbind();
	}

public:
	Mesh(
		std::vector<Vertex> aVertices,
		std::vector<Texture*> aTextures,
		std::vector<uint32_t> aIndices
	) 
		: m_vertices{ std::move(aVertices) }
		, m_textures{ std::move(aTextures) }
		, m_indices{ std::move(aIndices) }
	{
		setupMesh();
	}

	// I think we need the copy and move constructor here... WHen we push back into a vector, how does it know 
	// what to put in for the object? there is no obvious answer with all our pointers

	// No! We can only use a move constructor because unique pointers can only be moved. I think this feels correct.
	// The mesh is sent to be owned by the enclosing vector and hence by the parent model class

	Mesh(Mesh&& other)
		: m_VAO{ std::move(other.m_VAO) }
		, m_EBO{ std::move(other.m_EBO) }
		, m_VBO{ std::move(other.m_VBO) }
		, m_vertices { std::move(other.m_vertices) }
		, m_textures { std::move(other.m_textures) }
		, m_indices { std::move(other.m_indices) }
	{
	}

	~Mesh() {}

	void draw(Shader* aShader)
	{
		auto diffuse_cnt = uint32_t{0};
		auto specular_cnt = uint32_t{0};

		for (uint32_t i = 0; i < m_textures.size(); ++i)
		{
			auto unit = GL_TEXTURE0 + i;
			
			auto name = std::string{};
			if (m_textures[i]->getType() == SPECULAR)
			{
				name = "specular" + std::to_string(specular_cnt++);
			}
			else if (m_textures[i]->getType() == DIFFUSE)
			{
				name = "diffuse" + std::to_string(diffuse_cnt++);
			}
			aShader->set_uniformi(name.c_str(), i);
			m_textures[i]->use(unit);
		}

		m_VAO->bind();
		glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(m_indices.size()), GL_UNSIGNED_INT, 0);
		m_VAO->unbind();

		glActiveTexture(GL_TEXTURE0);
	}
};
