#pragma once

#include <memory>
#include <string>

#include "Camera.h"
#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

class Window
{
protected:
	unsigned int m_width;
	unsigned int m_height;

	bool m_should_close;

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Renderer> m_renderer;

public:
	Window() :
		m_width{ SCREEN_WIDTH },
		m_height{ SCREEN_HEIGHT }
	{
		m_camera = std::unique_ptr<Camera>(new Camera(m_width, m_height));
		m_renderer = std::make_unique<Renderer>();
	}

	~Window() {}
	
	// run before the game loop 
	void on_load() 
	{
	}
	
	void on_update_frame() {}
	
	// run after on_update_frame to render state
	void on_render_frame() {}
	
	// called whenever the window is resized
	virtual void on_resize(int width, int height)
	{
		m_width = width;
		m_height = height;
		m_camera.get()->process_resize(width, height);
		glViewport(0, 0, width, height);
	}
	
	// destroy any state with this window
	void on_unload() {}

	bool should_close() const
	{
		return m_should_close;
	}
};
