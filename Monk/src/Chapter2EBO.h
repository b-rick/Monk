#pragma once

#include "Box.h"

#include "Shader.h"
#include "Window.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "ElementBuffer.h" 

class Chapter2EBO : public Window
{
private:
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<ElementBuffer> m_EBO;
	std::unique_ptr<VertexBuffer> m_VBO;

	std::unique_ptr<Shader> m_Shader;

	bool m_wireframe;
	bool m_last_wireframe;
	bool m_window_open;
public:
	Chapter2EBO() : Window() 
	{
		m_last_wireframe = false;
		m_wireframe = false;
		m_window_open = false;
	}

	~Chapter2EBO() {}

	void on_load()
	{
		m_VBO = std::make_unique<VertexBuffer>(TRIANGLE_VERT, static_cast<uint32_t>(sizeof(TRIANGLE_VERT)), GL_STATIC_DRAW);
		m_EBO = std::make_unique<ElementBuffer>(TRIANGLE_INDICES, static_cast<uint32_t>(sizeof(TRIANGLE_INDICES)), GL_STATIC_DRAW);
		
		auto triangle_vbl = VertexBufferLayout();
		triangle_vbl.push<float>(3);
		m_VAO = std::make_unique<VertexArray>();
		m_VAO->add_element_buffer(m_VBO.get(), m_EBO.get(), triangle_vbl);

		m_Shader = std::make_unique<Shader>("shader/triangle/triangle.vert", "shader/triangle/triangle.frag");
		
	}

	void on_update_frame()
	{

	}

	void on_render_frame()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_Shader->use();
		m_VAO->bind();
		
		if (m_wireframe)
		{
			if (m_last_wireframe != m_wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			m_last_wireframe = m_wireframe;
		}
		else 
		{
			if (m_last_wireframe != m_wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			m_last_wireframe = m_wireframe;
		}
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Triangle wireframe ", &m_window_open);
		ImGui::Checkbox("Wireframe enabled", &m_wireframe);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
};
