#include "DebugWindow.h"

#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class PauseMenu
{
private:
	bool m_is_open;
	ImVec2 m_button_sz;
	std::string m_title;

	DebugWindow* m_debug_window;

public:
	PauseMenu(DebugWindow* a_debug_window) 
		: m_is_open{ false }
		, m_title{ "Paused" }
		, m_debug_window{ a_debug_window }
		, m_button_sz{ ImVec2(150, 0) } 
	{}

	void render(uint32_t x, uint32_t y);

	bool is_paused() const;
};