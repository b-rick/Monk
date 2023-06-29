#pragma once

#include <memory>

#include "Shader.h"
#include "TextWidget.h"

class AutoShader
{
private:
	std::string m_vertex_src;
	std::string m_fragment_src;

	std::unique_ptr<Shader> m_shader;

	bool m_compile_status;
	std::string m_reason;

public:
	AutoShader() 
		: m_vertex_src {}
		, m_fragment_src {}
		, m_compile_status{ false }
	{
	}

	AutoShader(const char* vertex_src, const char* fragment_src)
		: m_vertex_src{ vertex_src }
		, m_fragment_src{ fragment_src }
		, m_compile_status{ false }
	{
	}
	
	void try_compile();

	void update_vertex_src(const char* vertex_src);

	void update_fragment_src(const char* fragment_src);

	bool get_status() const;
};
