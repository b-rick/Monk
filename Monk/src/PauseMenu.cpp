#include "PauseMenu.h"

void PauseMenu::render(uint32_t x, uint32_t y)
{
	if (ImGui::IsKeyPressed(ImGuiKey_Escape))
	{
		m_is_open = !m_is_open;
	}

	if (!m_is_open)
	{
		return;
	}
	
	m_debug_window->log_info("is_open: %s\n", m_is_open ? "true" : "false");
	auto vp = ImGui::GetMainViewport();
	auto work_pos = vp->WorkPos;
	auto work_size = vp->WorkSize;

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
	ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(work_size.x / 2, work_size.y / 2), ImGuiCond_Always);

	if (!ImGui::Begin(m_title.c_str(), &m_is_open, flags))
	{
		ImGui::End();
		return;
	}

	auto window_sz = ImGui::GetWindowSize();
	auto center_pos = ImVec2{ (window_sz.x - m_button_sz.x) / 2, (window_sz.y - m_button_sz.y) / 2 };
	ImGui::SetCursorPos(center_pos);

	if (ImGui::Button("Close pause", m_button_sz))
	{
		m_is_open = false;
	}
	ImGui::End();
}

bool PauseMenu::is_paused() const
{
	return m_is_open;
}
