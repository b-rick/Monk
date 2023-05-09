#pragma once

#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>

class Shader
{
private:
	unsigned int m_Handle;

	std::string slurp(std::ifstream& aIn)
	{
		auto sstr = std::ostringstream{};
		sstr << aIn.rdbuf();
		return sstr.str();
	}

	unsigned int compileShader(const char* aShaderPath, GLenum aShaderType)
	{
		auto shaderStream = std::ifstream{ aShaderPath };
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
		auto shaderSrcC = shaderSrc.c_str();

		auto const shader = glCreateShader(aShaderType);

		glShaderSource(shader, 1, &shaderSrcC, nullptr);
		glCompileShader(shader);

		handleCompileError(shader, GL_COMPILE_STATUS);
		return shader;
	}

	void handleCompileError(unsigned int aId, GLenum aStatus)
	{
		auto success = int{};
		glGetShaderiv(aId, aStatus, &success);
		if (success == 0)
		{
			char infoLog[512]{};
			glGetShaderInfoLog(aId, 512, nullptr, infoLog);
			std::cerr << "SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	void handleLinkError(unsigned int aId, GLenum aStatus)
	{
		auto success = int{};
		glGetProgramiv(aId, aStatus, &success);
		if (success == 0)
		{
			char infoLog[512]{};
			glGetProgramInfoLog(aId, 512, nullptr, infoLog);
			std::cerr << "SHADER::LINK_FAILED\n" << infoLog << std::endl;
		}
	}

public:
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		auto vertexShader = compileShader(vertexPath, GL_VERTEX_SHADER);
		auto fragmentShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

		m_Handle = glCreateProgram();
		glAttachShader(m_Handle, vertexShader);
		glAttachShader(m_Handle, fragmentShader);
		glLinkProgram(m_Handle);
		
		handleLinkError(m_Handle, GL_LINK_STATUS);

		glDetachShader(m_Handle, vertexShader);
		glDetachShader(m_Handle, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
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
