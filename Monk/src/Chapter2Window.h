#pragma once

#include "Box.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Locations.h"
#include "LightingWindow.h"
#include "Window.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>

class Chapter2Window : public Window
{
private:

	std::unique_ptr<LightingWindow> m_light_window;

	std::unique_ptr<VertexBuffer> m_vertex_buffer;
	std::unique_ptr<VertexArray> m_box_vao;
	std::unique_ptr<VertexArray> m_light_vao;

	std::unique_ptr<Shader> m_box_shader;
	std::unique_ptr<Shader> m_cube_shader;
	
	std::unique_ptr<Texture> m_box_texture;
	std::unique_ptr<Texture> m_box_spec_texture;
public:
	Chapter2Window() : Window()
	{
		m_light_window = std::unique_ptr<LightingWindow>(new LightingWindow());
	}

    void on_update_frame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        auto& io = ImGui::GetIO(); IM_UNUSED(io);
        auto vp = ImGui::GetMainViewport();

        if (!m_light_window->is_window_open())
        {
            m_camera->process_mouse_movement(io.MouseDelta.x, -1.0f * io.MouseDelta.y);
            m_camera->process_mouse_scroll(io.MouseWheel);
        }

        if (ImGui::IsKeyDown(ImGuiKey_Escape))
        {
            m_light_window->set_window_open(true);
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

        if (m_light_window->should_close_app())
        {
            m_should_close = true;
        }

        m_light_window->draw_window();
        ImGui::Render();
    }

    void on_render_frame()
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto view = m_camera->view_matrix();
        auto project = m_camera->projection_matrix();

        // set the flash light uniforms
        m_box_shader->use();
        m_box_shader->set_uniform_vec3("flashlight.position", m_camera->position());
        m_box_shader->set_uniform_vec3("flashlight.direction", m_camera->front());

        // draw boxes
        m_box_shader->set_uniform_vec3("viewPos", m_camera->position());
        m_box_shader->set_uniform_mat4("view", view);
        m_box_shader->set_uniform_mat4("projection", project);

        // direction light
        m_box_shader->set_uniform_vec3("directionLight.direction", m_light_window->direction_light_direction());
        m_box_shader->set_uniform_vec3("directionLight.ambient", m_light_window->direction_light_ambient());
        m_box_shader->set_uniform_vec3("directionLight.diffuse", m_light_window->direction_light_diffuse());
        m_box_shader->set_uniform_vec3("directionLight.specular", m_light_window->direction_light_specular());

        // point lights
        for (uint32_t i = 0; i < std::size(light_positions); ++i)
        {
            auto point_light_attrib_name = "pointLights[" + std::to_string(i) + "]";
            m_box_shader->set_uniform_vec3((point_light_attrib_name + std::string{ ".position" }).c_str(), light_positions[i]);

            m_box_shader->set_uniform_vec3((point_light_attrib_name + std::string{ ".ambient" }).c_str(), m_light_window->point_light_ambient());
            m_box_shader->set_uniform_vec3((point_light_attrib_name + std::string{ ".diffuse" }).c_str(), m_light_window->point_light_diffuse());
            m_box_shader->set_uniform_vec3((point_light_attrib_name + std::string{ ".specular" }).c_str(), m_light_window->point_light_specular());

            m_box_shader->set_float((point_light_attrib_name + std::string{ ".attn_constant" }).c_str(), m_light_window->point_light_attn_const());
            m_box_shader->set_float((point_light_attrib_name + std::string{ ".attn_linear" }).c_str(), m_light_window->point_light_attn_linear());
            m_box_shader->set_float((point_light_attrib_name + std::string{ ".attn_quad" }).c_str(), m_light_window->point_light_attn_quad());
        }

        // spot lights
        m_box_shader->set_uniform_vec3("flashlight.ambient", m_light_window->flash_light_ambient());
        m_box_shader->set_uniform_vec3("flashlight.diffuse", m_light_window->flash_light_diffuse());
        m_box_shader->set_uniform_vec3("flashlight.specular", m_light_window->flash_light_specular());

        m_box_shader->set_float("flashlight.attn_constant", m_light_window->flash_light_attn_const());
        m_box_shader->set_float("flashlight.attn_linear", m_light_window->flash_light_attn_linear());
        m_box_shader->set_float("flashlight.attn_quad", m_light_window->flash_light_attn_quad());

        m_box_shader->set_float("flashlight.cutoff", m_light_window->flash_light_cutoff());
        m_box_shader->set_float("flashlight.outerCutoff", m_light_window->flash_light_outer_cutoff());

        m_box_vao->bind();
        for (uint32_t i = 0; i < std::size(cube_positions); ++i)
        {
            auto cube_model = glm::translate(glm::mat4(1.0f), cube_positions[i]);
            float angle = 20.0f * i;
            cube_model = glm::rotate(cube_model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            m_box_shader->set_uniform_mat4("model", cube_model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // draw lights 
        m_cube_shader->use();
        m_cube_shader->set_uniform_mat4("view", view);
        m_cube_shader->set_uniform_mat4("projection", project);
        m_cube_shader->set_uniform_vec3("lightColor", m_light_window->point_light_specular()); // white light

        m_light_vao->bind();
        for (uint32_t i = 0; i < std::size(light_positions); ++i)
        {
            auto light_model = glm::mat4(1.0f);
            light_model = glm::translate(light_model, light_positions[i]);
            light_model = glm::scale(light_model, glm::vec3(0.2f));
            m_cube_shader->set_uniform_mat4("model", light_model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

	void on_load()
	{

        // Box VBO
        auto sz = sizeof(BOX);
        m_vertex_buffer = std::unique_ptr<VertexBuffer>(new VertexBuffer(BOX, sizeof(BOX), GL_STATIC_DRAW));

        // Container VAO 
        auto box_vbl = VertexBufferLayout();
        box_vbl.push<float>(3); // (vertices) 
        box_vbl.push<float>(3); // (normals) 
        box_vbl.push<float>(2); // (tex coords)
        m_box_vao = std::unique_ptr<VertexArray>(new VertexArray());
        m_box_vao->addBuffer(m_vertex_buffer.get(), box_vbl);

        // Point light VAO
        auto light_vbl = VertexBufferLayout();
        light_vbl.push<float>(3); // (vertices) no need for normals
        light_vbl.push_empty<float>(5); // j
        m_light_vao = std::unique_ptr<VertexArray>(new VertexArray());
        m_light_vao->addBuffer(m_vertex_buffer.get(), light_vbl);
        
        // Shaders
        m_box_shader = std::unique_ptr<Shader>(new Shader("shader/multilight/colors.vert", "shader/multilight/colors.frag"));
        m_cube_shader = std::unique_ptr<Shader>(new Shader("shader/multilight/lightCube.vert", "shader/multilight/lightCube.frag"));
        
        // Textures
        m_box_texture = std::unique_ptr<Texture>(new Texture("img/container2.png", DIFFUSE));
        m_box_spec_texture = std::unique_ptr<Texture>(new Texture("img/container2_specular.png", DIFFUSE));


        // Container material 
        m_box_shader->use();
        m_box_shader->set_uniformi("material.diffuse", 0);
        m_box_shader->set_uniformi("material.specular", 1);
        m_box_shader->set_float("material.shininess", 32.0f);
        m_box_texture->use(GL_TEXTURE0);
        m_box_spec_texture->use(GL_TEXTURE1);

        // point light cube shaders
        m_cube_shader->use();
	}

    void on_unload() {}
};
