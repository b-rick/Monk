#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include <vector>

class DebugWindow
{
private:
	bool m_window_open;
	bool m_border_shown;
	const char* m_title;
	bool m_auto_scroll;
	bool m_paused;

	ImGuiTextBuffer m_buffer;
	ImGuiTextFilter m_filter;
	std::vector<int> m_line_offsets;

	void add_log(const char* fmt, va_list args) IM_FMTARGS(2);
public:
	DebugWindow(const char* title) : m_window_open{ false }, m_title{ title }, m_auto_scroll{ true }, m_border_shown{ false }, m_paused{ false } 
	{
		clear();
	};

	void render();

	void clear();

	void log_info(const char* fmt, ...) IM_FMTARGS(2);

	void log_debug(const char* fmt, ...) IM_FMTARGS(2);

	void log_warn(const char* fmt, ...) IM_FMTARGS(2);

	void log_error(const char* fmt, ...) IM_FMTARGS(2);

};
