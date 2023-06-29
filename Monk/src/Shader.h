#pragma once


#include "io/IOUtils.h"
#include <functional>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define NOT_READY UINT32_MAX

struct ShaderErrorCallback
{
	using Callback = std::function<void(char*)>;
	ShaderErrorCallback(Callback fn) : fn_{ std::move(fn) } {}
	Callback fn_;

	void operator() (char * err)
	{
		fn_(err);
	}
};

class Shader
{
private:
	unsigned int m_Handle;

	static unsigned int compileShaderSource(const char* shader_src, GLenum aShaderType, ShaderErrorCallback error_callback)
	{
		auto const shader = glCreateShader(aShaderType);

		glShaderSource(shader, 1, &shader_src, nullptr);
		glCompileShader(shader);

		handleCompileError(shader, GL_COMPILE_STATUS, error_callback);
		return shader;
	}

	static void handleCompileError(unsigned int aId, GLenum aStatus, ShaderErrorCallback error_callback)
	{
		auto success = int{};
		glGetShaderiv(aId, aStatus, &success);
		if (success == 0)
		{
			char infoLog[512]{};
			glGetShaderInfoLog(aId, 512, nullptr, infoLog);
			error_callback(infoLog);
		}
	}

	void handleLinkError(unsigned int aId, GLenum aStatus, ShaderErrorCallback error_callback)
	{
		auto success = int{};
		glGetProgramiv(aId, aStatus, &success);
		if (success == 0)
		{
			char infoLog[512]{};
			glGetProgramInfoLog(aId, 512, nullptr, infoLog);
			error_callback(infoLog);
		}
	}

	void init(int vertex_shader, int fragment_shader, ShaderErrorCallback err_cb)
	{
		m_Handle = glCreateProgram();
		glAttachShader(m_Handle, vertex_shader);
		glAttachShader(m_Handle, fragment_shader);
		glLinkProgram(m_Handle);

		handleLinkError(m_Handle, GL_LINK_STATUS, err_cb);

		glDetachShader(m_Handle, vertex_shader);
		glDetachShader(m_Handle, fragment_shader);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}

public:
	explicit Shader(int vertex_shader, int fragment_shader, ShaderErrorCallback err_cb)
	{
		init(vertex_shader, fragment_shader, err_cb);
	}

	Shader() : m_Handle{ NOT_READY }
	{
	}

	Shader(const char* vertexPath, const char* fragmentPath) 
	{
		auto my_error_cb = ShaderErrorCallback{ [](char* infoLog) { std::cerr << "SHADER::COMPILATION_FAILED\n" << infoLog << std::endl; } };

		auto vert_src = IOUtils::read_src(vertexPath);
		auto frag_src = IOUtils::read_src(fragmentPath);
		
		auto vert_shader = compileShaderSource(vert_src.c_str(), GL_VERTEX_SHADER, my_error_cb);
		auto frag_shader = compileShaderSource(frag_src.c_str(), GL_FRAGMENT_SHADER, my_error_cb);

		init(vert_shader, frag_shader, my_error_cb);
	}

	Shader& operator=(Shader&& shader) noexcept
	{
		m_Handle = shader.m_Handle;
		shader.m_Handle = NOT_READY;
		return *this;
	}

	static Shader fromText(const char* vertex_src, const char* frag_src, ShaderErrorCallback err_callback)
	{
		auto vertex_shader = compileShaderSource(vertex_src, GL_VERTEX_SHADER, err_callback);
		auto fragment_shader = compileShaderSource(frag_src, GL_FRAGMENT_SHADER, err_callback);
		return Shader(vertex_shader, fragment_shader, err_callback);
	}

	void use() const
	{
		if (m_Handle == NOT_READY)
		{
			// TODO: Move this to our debug logger after it's been made static. Maybe even a logger factory? 
			std::cerr << "Do not try to use the default shader!" << std::endl;
		}
		else 
		{
			glUseProgram(m_Handle);
		}
	}

	inline void set_uniformi(const char* attribName, unsigned int value) const
	{
		int location = get_uniform_location(attribName);
		glUniform1i(location, value);
	}

	inline void set_float(const char* attribName, float value) const
	{
		int location = get_uniform_location(attribName);
		glUniform1f(location, value);
	}

	inline void set_uniform_vec3(const char* attribName, const glm::vec3& vec3) const
	{
		int location = get_uniform_location(attribName);
		glUniform3fv(location, 1, &vec3[0]);
	}

	inline void set_uniform_vec4(const char* attribName, const glm::vec4& vec4) const
	{
		int location = get_uniform_location(attribName);
		glUniform4fv(location, 1, &vec4[0]);
	}

	inline void set_uniform_mat4(const char* attribName, const glm::mat4& mat4) const
	{
		int location = get_uniform_location(attribName);
		glUniformMatrix4fv(location, 1, GL_FALSE, &mat4[0][0]);
	}

	inline int get_uniform_location(const char* attribName) const
	{
		if (m_Handle == NOT_READY)
		{
			return -1;
		} 
		else
		{
			return glGetUniformLocation(m_Handle, attribName);
		}
	}

	~Shader()
	{
		if (m_Handle != NOT_READY)
		{
			glDeleteProgram(m_Handle);
			// TODO: Move this to our debug logger after it's been made static. Maybe even a logger factory? 
			std::cerr << "Deleting the shader" << std::endl;
		}
	}
};
