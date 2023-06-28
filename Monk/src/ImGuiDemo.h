#pragma once

#include "AutoShader.h"
#include "Box.h"
#include "Framebuffer.h"
#include "ColorAttachment.h"
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


class ImGuiDemo : public Window
{
private:
    bool m_window_open;
   
public:
    ImGuiDemo()
        : Window()
        , m_window_open{true}
    {
    }

    void on_update_frame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        auto& io = ImGui::GetIO(); IM_UNUSED(io);
        auto vp = ImGui::GetMainViewport();

        ImGui::ShowDemoWindow(&m_window_open);
        ImGui::Render();
    }

    void on_load()
    {
    }

    void on_render_frame()
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};
