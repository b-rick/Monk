#pragma once

#include "Box.h"
#include "Locations.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Window.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define NUM_SHADERS 3

const char* depth_test[] = { "NEVER", "LESS", "EQUAL", "LEQUAL", "GREATER", "NOT_EQUAL", "GEQUAL", "ALWAYS" };
const char* shader_selection[] = { "NORMAL", "NON_LINEAR_DEPTH_VIZ", "LINEAR_DEPTH_VIZ" };

class Chapter4Depth : public Window
{
private:
	std::unique_ptr<VertexBuffer> m_box_buffer;
	std::unique_ptr<VertexBuffer> m_floor_buffer;

	std::unique_ptr<VertexArray> m_box_vao;
	std::unique_ptr<VertexArray> m_floor_vao;
	
    std::array<std::unique_ptr<Shader>, std::size(shader_selection)> m_shaders;
	std::unique_ptr<Texture> m_box_texture;
	std::unique_ptr<Texture> m_floor_texture;
    
    int m_current_shader;
    int m_current_depth_test;
    int m_previous_depth_test;
    
    float m_movement_speed;
    float m_near_plane;
    float m_far_plane;

    bool m_menu_open;

public:
	Chapter4Depth() : Window()
	{
        this->m_current_shader = 0;
        this->m_current_depth_test = 0;
        this->m_previous_depth_test = 0;

        this->m_movement_speed = 2.0;
        this->m_near_plane = 0.1f;
        this->m_far_plane = 100.0f;

        this->m_menu_open = false;
	}

    void on_load()
    {
        // make the box vao 
        m_box_buffer = std::make_unique<VertexBuffer>(BOX, static_cast<uint32_t>(sizeof(BOX)), GL_STATIC_DRAW);
        auto vbl = VertexBufferLayout();
        vbl.push<float>(3); // position
        vbl.push<float>(3); // normal
        vbl.push<float>(2); // texture
        
        m_box_vao = std::make_unique<VertexArray>();
        m_box_vao->addBuffer(m_box_buffer.get(), vbl);

        // max the floor vao
        m_floor_buffer = std::make_unique<VertexBuffer>(FLOOR, static_cast<uint32_t>(sizeof(FLOOR)), GL_STATIC_DRAW);
        vbl = VertexBufferLayout();
        vbl.push<float>(3); // position
        vbl.push<float>(3); // position
        vbl.push<float>(2); // texture

        m_floor_vao = std::make_unique<VertexArray>();
        m_floor_vao->addBuffer(m_floor_buffer.get(), vbl);

        m_box_texture = std::make_unique<Texture>("img/depth/marble.jpg", DIFFUSE);
        m_floor_texture = std::make_unique<Texture>("img/depth/metal.png", DIFFUSE);
        
        m_shaders =
        {
            std::make_unique<Shader>("shader/depth/normal.vert", "shader/depth/normal.frag"),
            std::make_unique<Shader>("shader/depth/normal.vert", "shader/depth/depth-viz.frag"),
            std::make_unique<Shader>("shader/depth/normal.vert", "shader/depth/depth-viz-linear.frag")
        };
        
        m_shaders[m_current_shader]->set_uniformi("texture0", 0);
    }

    void on_update_frame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        auto& io = ImGui::GetIO(); IM_UNUSED(io);
        auto vp = ImGui::GetMainViewport();
        
        if (m_menu_open)
        {
			ImGui::Begin("Depth testing", &m_menu_open);
			ImGui::Combo("Test function", &m_current_depth_test, depth_test, IM_ARRAYSIZE(depth_test));
            ImGui::Combo("Shader selection", &m_current_shader, shader_selection, IM_ARRAYSIZE(shader_selection));
            ImGui::SliderFloat("Movement speed", &m_movement_speed, 1.0f, 5.0f);
            ImGui::SliderFloat("Near plane", &m_near_plane, 0.1f, 10.0f);
            ImGui::SliderFloat("Far plane", &m_far_plane, 20.0f, 500.0f);
            if (ImGui::Button("Close app"))
            {
                m_should_close = true;
            }
			ImGui::End();
        }
        else
        {
			m_camera->process_mouse_movement(io.MouseDelta.x, -1.0f * io.MouseDelta.y);
			m_camera->process_mouse_scroll(io.MouseWheel);
        }

        if (ImGui::IsKeyDown(ImGuiKey_Escape))
        {
            m_menu_open = true;
        }
        if (ImGui::IsKeyDown(ImGuiKey_W))
        {
            m_camera->process_keypress(CameraMovement::FORWARD, io.DeltaTime * m_movement_speed);
        }
        if (ImGui::IsKeyDown(ImGuiKey_S))
        {
            m_camera->process_keypress(CameraMovement::BACKWARD, io.DeltaTime * m_movement_speed);
        }
        if (ImGui::IsKeyDown(ImGuiKey_A))
        {
            m_camera->process_keypress(CameraMovement::LEFT, io.DeltaTime * m_movement_speed);
        }
        if (ImGui::IsKeyDown(ImGuiKey_D))
        {
            m_camera->process_keypress(CameraMovement::RIGHT, io.DeltaTime * m_movement_speed);
        }
        if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
        {
            m_camera->process_keypress(CameraMovement::DOWN, io.DeltaTime * m_movement_speed);
        }
        if (ImGui::IsKeyDown(ImGuiKey_Space))
        {
            m_camera->process_keypress(CameraMovement::UP, io.DeltaTime * m_movement_speed);
        }

        m_camera->set_near_plane(m_near_plane);
        m_camera->set_far_plane(m_far_plane);

        ImGui::Render();
    }

    void on_render_frame()
    {
        if (m_current_depth_test != m_previous_depth_test)
        {
            glDepthFunc(GL_NEVER + m_current_depth_test);
            m_previous_depth_test = m_current_depth_test;
        }
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto view = m_camera->view_matrix();
        auto project = m_camera->projection_matrix();

        m_shaders[m_current_shader]->use();
        m_shaders[m_current_shader]->set_uniform_mat4("view", view);
        m_shaders[m_current_shader]->set_uniform_mat4("projection", project);
        m_box_texture->use(GL_TEXTURE0);

        m_box_vao->bind();
        for (uint32_t i = 0; i < 2; ++i)
        {
            auto cube_model = glm::translate(glm::mat4(1.0f), glm::vec3(i * 5, 0.0f, 0.0f));
            m_shaders[m_current_shader]->set_uniform_mat4("model", cube_model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        m_floor_texture->use(GL_TEXTURE0);
        m_shaders[m_current_shader]->set_uniform_mat4("model", glm::mat4(1.0f));
        m_floor_vao->bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};
