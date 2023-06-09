#pragma once

#include "AutoShader.h"
#include "AutoShaderMenu.h"
#include "Box.h"
#include "DebugWindow.h"
#include "Framebuffer.h"
#include "ColorAttachment.h"
#include "PauseMenu.h"
#include "RenderBuffer.h"
#include "Locations.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Window.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include "TextWidget.h"


class Chapter4FB : public Window
{
private:
	bool m_menu_open;

    float m_movement_speed;

    std::unique_ptr<VertexBuffer> m_box_buffer;
    std::unique_ptr<VertexBuffer> m_floor_buffer;
    std::unique_ptr<VertexBuffer> m_quad_buffer;

    std::unique_ptr<VertexArray> m_box_vao;
    std::unique_ptr<VertexArray> m_floor_vao;
    std::unique_ptr<VertexArray> m_quad_vao;

    std::unique_ptr<Shader> m_draw_shader;
    std::unique_ptr<Shader> m_frame_shader;

    std::unique_ptr<Texture> m_box_texture;
    std::unique_ptr<Texture> m_floor_texture;
    
    std::unique_ptr<FrameBuffer> m_framebuffer;
    std::unique_ptr<ColorTextureAttachment> m_color_attachment;
    std::unique_ptr<RenderBuffer> m_depth_stencil_rb;
    
    std::unique_ptr<DebugWindow> m_debug_window;
    AutoShaderMenu m_auto_shader_menu;
    AutoShader m_auto_shader;
    PauseMenu m_pause_menu;
    
public:
	Chapter4FB() 
        : Window()
        , m_menu_open{false}
        , m_movement_speed{2.0}
        , m_debug_window{ std::make_unique<DebugWindow>("debug window") }
        , m_auto_shader_menu{ m_debug_window.get()}
        , m_pause_menu{ m_debug_window.get() }
	{
	}

	void on_update_frame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();
		auto& io = ImGui::GetIO(); IM_UNUSED(io);
		auto vp = ImGui::GetMainViewport();

        m_auto_shader_menu.render();

        auto pause_x = m_auto_shader_menu.width() + m_auto_shader_menu.remaining_width() / 2;
        auto pause_y = m_auto_shader_menu.height() / 2;
        m_pause_menu.render(pause_x, pause_y);
        m_debug_window->render();
        
        if (!m_pause_menu.is_paused())
        {
			m_camera->process_mouse_movement(io.MouseDelta.x, -1.0f * io.MouseDelta.y);
			m_camera->process_mouse_scroll(io.MouseWheel);

			if (ImGui::IsKeyDown(ImGuiKey_Escape))
			{

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
        }
        ImGui::Render();
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

        // make the floor vao
        m_floor_buffer = std::make_unique<VertexBuffer>(FLOOR, static_cast<uint32_t>(sizeof(FLOOR)), GL_STATIC_DRAW);
        vbl = VertexBufferLayout();
        vbl.push<float>(3); // position
        vbl.push<float>(3); // position
        vbl.push<float>(2); // texture
        m_floor_vao = std::make_unique<VertexArray>();
        m_floor_vao->addBuffer(m_floor_buffer.get(), vbl);

        // make the quad vao for rendering our fb 
        m_quad_buffer = std::make_unique<VertexBuffer>(QUAD_NDC, static_cast<uint32_t>(sizeof(QUAD_NDC)), GL_STATIC_DRAW);
        vbl = VertexBufferLayout();
        vbl.push<float>(2); // position
        vbl.push<float>(2); // texture
        m_quad_vao = std::make_unique<VertexArray>();
        m_quad_vao->addBuffer(m_quad_buffer.get(), vbl);


        m_draw_shader = std::make_unique<Shader>("shader/fb/normal.vert", "shader/fb/normal.frag");
        m_frame_shader = std::make_unique<Shader>("shader/fb/frame.vert", "shader/fb/frame.frag");

        auto vertex_src = IOUtils::read_src("shader/fb/frame.vert");
        auto fragment_src = IOUtils::read_src("shader/fb/frame.frag");
        m_auto_shader.update_vertex_src(vertex_src.c_str());
        m_auto_shader.update_fragment_src(fragment_src.c_str());
        m_auto_shader.try_compile();

        m_box_texture = std::make_unique<Texture>("img/container.jpg", DIFFUSE);
        m_floor_texture = std::make_unique<Texture>("img/depth/metal.png", DIFFUSE);

        /* Create the framebuffer */
        m_framebuffer = std::make_unique<FrameBuffer>();
        m_color_attachment = std::make_unique<ColorTextureAttachment>(m_width, m_height);
        m_depth_stencil_rb = std::make_unique<RenderBuffer>(GL_DEPTH24_STENCIL8, m_width, m_height);

        m_framebuffer->attach_color_texture(*m_color_attachment);
        m_framebuffer->attach_render_buffer(*m_depth_stencil_rb);

        auto status = FrameBuffer::check_status();
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }
        m_framebuffer->unbind();
    }

    void on_resize(int width, int height)
    {
        Window::on_resize(width, height);
        m_color_attachment->resize(width, height);
        m_depth_stencil_rb->resize(width, height);

        auto status = FrameBuffer::check_status();
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }
        m_framebuffer->unbind();
    }

	void on_render_frame()
	{
        m_framebuffer->bind();
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_SCISSOR_TEST);
        glScissor(m_auto_shader_menu.width(), 0, m_auto_shader_menu.remaining_width(), m_auto_shader_menu.height());

        auto view = m_camera->view_matrix();
        auto project = m_camera->projection_matrix();

        /** Draw to the new framebuffer! */
        
        /* Start with just our usual draw shader */
        m_draw_shader->use();
        m_draw_shader->set_uniform_mat4("view", view);
        m_draw_shader->set_uniform_mat4("projection", project);
        
        /* ====================== DRAW FLOOR =================== */
        m_draw_shader->set_uniform_mat4("model", glm::mat4(1.0f));
        m_floor_texture->use(GL_TEXTURE0);
        m_floor_vao->bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        /* ====================== DRAW BOXES =================== */
        m_box_texture->use(GL_TEXTURE0);
        m_box_vao->bind();
        for (uint32_t i = 0; i < 2; ++i)
        {
            auto cube_model = glm::translate(glm::mat4(1.0f), glm::vec3(i * 3.0f, 0.0f, i * 3.0f));
            m_draw_shader->set_uniform_mat4("model", cube_model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glDisable(GL_SCISSOR_TEST);

        /** Bind back to the default framebuffer and draw! */
        FrameBuffer::bind_default();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT); // we need to clear the buffers after the previous fb is drawn
        m_frame_shader->use();
        m_quad_vao->bind();
        glDisable(GL_DEPTH_TEST); // disable the depth test because we need the quad to be drawn in front of everything else
        m_color_attachment->bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
};
