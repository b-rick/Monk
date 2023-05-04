#pragma once

#include "Model.h"
#include "Window.h"
#include <sbti/stb_image.h>


class Chapter3Mesh : public Window
{
private:
	std::unique_ptr<Model> m_Model;
	std::unique_ptr<Shader> m_Shader;

	bool window_open;
public:
	Chapter3Mesh() : Window() {}

	~Chapter3Mesh() {}

	// run before the game loop 
	void on_load() 
	{
		//stbi_set_flip_vertically_on_load(true);
		//m_Model = std::make_unique<Model>("objects/backpack/backpack.obj", m_renderer.get());
		m_Model = std::make_unique<Model>("objects/miffy/model.fbx", m_renderer.get());
		m_Shader = std::make_unique<Shader>("shader/model/model_loading.vs", "shader/model/model_loading.fs");
	}

	void on_update_frame() 
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		
		ImGui::NewFrame();
		auto& io = ImGui::GetIO(); IM_UNUSED(io);
		auto vp = ImGui::GetMainViewport();

		if (window_open)
		{
			ImGui::Begin("Lighting controller", &window_open);
			ImGui::Text("Directional light");
			if (ImGui::Button("Close Window"))
				window_open = false;
			ImGui::End();
		}
		ImGui::Render();

		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) 
		{
			m_camera->process_mouse_movement(io.MouseDelta.x, -1.0f * io.MouseDelta.y);
		}
		m_camera->process_mouse_scroll(io.MouseWheel);


		if (ImGui::IsKeyDown(ImGuiKey_Escape))
		{
			std::cerr << "called escape" << std::endl;
			m_should_close = true;
		}
		if (ImGui::IsKeyDown(ImGuiKey_W))
		{
			m_camera->process_keypress(CameraMovement::FORWARD, io.DeltaTime);
		}
		if (ImGui::IsKeyDown(ImGuiKey_S))
		{
			m_camera->process_keypress(CameraMovement::BACKWARD, io.DeltaTime);
		}
		if (ImGui::IsKeyDown(ImGuiKey_A))
		{
			m_camera->process_keypress(CameraMovement::LEFT, io.DeltaTime);
		}
		if (ImGui::IsKeyDown(ImGuiKey_D))
		{
			m_camera->process_keypress(CameraMovement::RIGHT, io.DeltaTime);
		}
		if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
		{
			m_camera->process_keypress(CameraMovement::DOWN, io.DeltaTime);
		}
		if (ImGui::IsKeyDown(ImGuiKey_Space))
		{
			m_camera->process_keypress(CameraMovement::UP, io.DeltaTime);
		}
	}

	// run after on_update_frame to render state
	void on_render_frame() 
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto projection = m_camera->projection_matrix();
		auto view = m_camera->view_matrix();

		m_Shader->use();
		m_Shader->set_uniform_mat4("projection", projection);
		m_Shader->set_uniform_mat4("view", view);

		auto model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.9f)); // scale down a bit
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // scale down a bit
		model = glm::translate(model, glm::vec3(-3.0f)); // transalte down to centre
		m_Shader->set_uniform_mat4("model", model);
		m_Model->Draw(m_Shader.get());
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	// called whenever the window is resized
	virtual void on_resize(int width, int height)
	{
		m_width = width;
		m_height = height;
		m_camera.get()->process_resize(width, height);
		glViewport(0, 0, width, height);
	}

	// destroy any state with this window
	void on_unload() {}

	bool should_close() const
	{
		return m_should_close;
	}
};
