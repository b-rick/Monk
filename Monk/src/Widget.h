#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>


#pragma once

class Widget
{
public:
	Widget();

	virtual ~Widget() = 0;

	virtual void setup() = 0;

	virtual void render() = 0;
};
