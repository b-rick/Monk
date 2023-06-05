#include "AutoShader.h"

void AutoShader::setup()
{
}

void AutoShader::render()
{
}

void AutoShader::try_compile()
{
	ShaderErrorCallback err_cb = {[=](char* result)
	{
		m_compile_status = false;
		m_reason = std::string{ result };
	}};

	auto vertex_txt = m_vertex_widget.get_text();
	auto frag_txt = m_fragment_widget.get_text();

	m_shader = std::make_unique<Shader>((Shader::fromText(vertex_txt.c_str(), frag_txt.c_str(), err_cb)));
	if (!m_compile_status)
	{
		std::cerr << "Failed to compile shader." <<  m_reason << std::endl;
	}
	else 
	{
		m_compile_status = true;
	}
}

bool AutoShader::get_status() const
{
	return m_compile_status;
}