#pragma once

#include <iostream>
#include <iterator>
#include <functional>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	static std::string slurp(std::ifstream& aIn)
	{
		auto sstr = std::ostringstream{};
		sstr << aIn.rdbuf();
		return sstr.str();
	}

	static std::string read_src(const char* path)
	{
		auto shaderStream = std::ifstream{ path };
		shaderStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		auto shaderSrc = std::string{};
		try
		{
			shaderSrc = slurp(shaderStream);
		}
		catch (std::ifstream::failure const& e)
		{
			std::cerr << "SHADER::FILE_NOT_READ\n" << e.what() << std::endl;
		}
		return shaderSrc;
	}

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

	Shader(const char* vertexPath, const char* fragmentPath) 
	{
		auto my_error_cb = ShaderErrorCallback{ [](char* infoLog) { std::cerr << "SHADER::COMPILATION_FAILED\n" << infoLog << std::endl; } };

		auto vert_src = read_src(vertexPath);
		auto frag_src = read_src(fragmentPath);
		
		auto vert_shader = compileShaderSource(vert_src.c_str(), GL_VERTEX_SHADER, my_error_cb);
		auto frag_shader = compileShaderSource(frag_src.c_str(), GL_FRAGMENT_SHADER, my_error_cb);

		init(vert_shader, frag_shader, my_error_cb);
	}

	static Shader fromText(const char* vertex_src, const char* frag_src, ShaderErrorCallback err_callback)
	{
		auto vertex_shader = compileShaderSource(vertex_src, GL_VERTEX_SHADER, err_callback);
		auto fragment_shader = compileShaderSource(frag_src, GL_FRAGMENT_SHADER, err_callback);
		return Shader(vertex_shader, fragment_shader, err_callback);
	}

	void use() const
	{
		glUseProgram(m_Handle);
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
		return glGetUniformLocation(m_Handle, attribName);
	}

	~Shader()
	{
		glDeleteProgram(m_Handle);
	}
};
