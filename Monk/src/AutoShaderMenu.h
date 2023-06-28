#pragma once

#include "DebugWindow.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include <memory>
#include <iostream>

const unsigned int DEFAULT_WIDTH = 1920;
const unsigned int DEFAULT_HEIGHT = 1080;

class AutoShaderMenu
{
private:
	bool m_window_open;
	bool m_loaded;

	float m_window_width;
	float m_window_height;

	DebugWindow* m_debug_window; // Should our logger be global? Maybe

	void render_contents();

public:
	AutoShaderMenu(DebugWindow* a_debug_window) 
		: m_window_open{ true }
		, m_debug_window {a_debug_window}
		, m_loaded{ false }
	{
	};

	void render();

	const uint32_t width() const;

	const uint32_t height() const;

	const uint32_t remaining_width() const;
};
