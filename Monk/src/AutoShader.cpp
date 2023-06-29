#include <algorithm>
#include "AutoShader.h"

void AutoShader::try_compile()
{
	ShaderErrorCallback err_cb = {[=](char* result)
	{
		m_compile_status = false;
		m_reason = std::string{ result };
	}};

	m_compile_status = true;
	m_shader = std::move(Shader::fromText(m_vertex_src.c_str(), m_fragment_src.c_str(), err_cb));
	if (!m_compile_status)
	{
		std::cerr << "Failed to create shader." <<  m_reason << std::endl;
	}
}

bool AutoShader::get_status() const
{
	return m_compile_status;
}

void AutoShader::update_vertex_src(const char* vertex_src)
{
	m_vertex_src = std::string{ vertex_src };
}

void AutoShader::update_fragment_src(const char* fragment_src)
{
	m_fragment_src = std::string{ fragment_src };
}

void AutoShader::use() const
{
	m_shader.use();
}
