#pragma once

#include <memory>

#include "Shader.h"
#include "TextWidget.h"

class AutoShader : Widget
{
private:
	TextWidget m_vertex_widget;
	TextWidget m_fragment_widget;

	bool m_success;
	std::string m_reason;

	std::unique_ptr<Shader> m_shader;

public:
	AutoShader(const char* header) 
		: m_vertex_widget{ TextWidget(header) }
		, m_fragment_widget{ TextWidget(header) }
		, m_success{ false }
	{
	}

	virtual void setup() override;

	virtual void render() override;
	
	void try_compile()
	{
		
		ShaderErrorCallback err_cb = { [=](char* result)
		{
			m_success = false;
			m_reason = std::string{ result };
		} };

		auto vertex_txt = m_vertex_widget.get_text();
		auto frag_txt = m_fragment_widget.get_text();
		
		auto from_text = std::make_unique<Shader>((Shader::fromText(vertex_txt.c_str(), frag_txt.c_str(), err_cb)));
		if (m_success)
		{
			std::cout << "woohoo!" << std::endl;
		}
	}
};
