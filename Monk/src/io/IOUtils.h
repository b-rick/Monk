#pragma once

#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>

namespace IOUtils
{
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
}
