#pragma once

#include <memory>

#include "Shader.h"
#include "TextWidget.h"

class AutoShader : Widget
{
private:
	std::unique_ptr<TextWidget> m_vertex_widget;
	std::unique_ptr<TextWidget> m_fragment_widget;

	std::unique_ptr<Shader> m_shader;

	bool m_compile_status;
	std::string m_reason;

public:
	AutoShader(std::unique_ptr<TextWidget> vertex_widget, std::unique_ptr<TextWidget> frag_widget)
		: m_vertex_widget{ std::move(vertex_widget) }
		, m_fragment_widget{ std::move(frag_widget) }
		, m_compile_status{ false }
	{
	}

	AutoShader() 
		: m_vertex_widget{ std::make_unique<TextWidget>("fragment")}
		, m_fragment_widget{ std::make_unique<TextWidget>("vertex")}
		, m_compile_status{ false }
	{
	}

	AutoShader(const char* vertex_src, const char* frag_src)
		: m_vertex_widget{ std::make_unique<TextWidget>("fragment")}
		, m_fragment_widget{ std::make_unique<TextWidget>("vertex")}
		, m_compile_status{ false }
	{
		m_vertex_widget->update_text(vertex_src);
		m_fragment_widget->update_text(frag_src);
	}

	virtual ~AutoShader() override {};

	virtual void setup() override;

	virtual void render() override;
	
	void try_compile();

	bool get_status() const;
};
