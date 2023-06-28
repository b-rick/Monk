#include "AutoShaderMenu.h"

void AutoShaderMenu::render_contents()
{
	ImGui::Text("Shader box");
	if (ImGui::Button("Open debug window"))
	{
		m_debug_window->open(true);
	}
}

void AutoShaderMenu::render()
{
	auto vp = ImGui::GetMainViewport();
	auto work_pos = vp->WorkPos;
	auto work_size = vp->WorkSize;

	if (!m_loaded) // runs on the first render
	{
		m_window_width = work_size.x / 4;
		m_loaded = true;
	}
	m_window_height = work_size.y; // unconditionally set height to be size of window

	ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1)); // Horizontal only;
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
	ImGui::SetNextWindowPos(work_pos, ImGuiCond_Always, ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(m_window_width, m_window_height), ImGuiCond_Always);
	if (ImGui::Begin("Shader input", & m_window_open, flags))
	{
		render_contents();
	}
	m_window_width = ImGui::GetWindowSize().x;
	ImGui::End();
}

const uint32_t AutoShaderMenu::width() const
{
	return static_cast<uint32_t>(m_window_width);
}

const uint32_t AutoShaderMenu::height() const
{
	return static_cast<uint32_t>(m_window_height);
}

const uint32_t AutoShaderMenu::remaining_width() const
{
	auto vp = ImGui::GetMainViewport();
	return static_cast<uint32_t>(vp->WorkSize.x) - width();
}
