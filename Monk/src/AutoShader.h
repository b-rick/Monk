#pragma once

#include <memory>

#include "Shader.h"
#include "TextWidget.h"

class AutoShader : Widget
{
private:
	TextWidget m_vertex_widget;
	TextWidget m_fragment_widget;

	bool m_compile_status;
	std::string m_reason;

	std::unique_ptr<Shader> m_shader;

public:
	AutoShader(const char* header) 
		: m_vertex_widget{ TextWidget(header) }
		, m_fragment_widget{ TextWidget(header) }
		, m_compile_status{ false }
	{
	}

	AutoShader(const char* header, const char* vertex_src, const char* frag_src)
		: m_vertex_widget{ TextWidget(header) }
		, m_fragment_widget{ TextWidget(header) }
		, m_compile_status{ false }
	{
		m_vertex_widget.update_text(vertex_src);
		m_fragment_widget.update_text(frag_src);
	}

	virtual ~AutoShader() override {};

	virtual void setup() override;

	virtual void render() override;
	
	void try_compile();

	bool get_status() const;
};
