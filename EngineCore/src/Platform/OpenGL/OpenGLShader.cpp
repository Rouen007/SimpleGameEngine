#include "sepch.h"
#include <fstream>
#include "OpenGLShader.h"
#include "SimpleEngine/Core/Log.h"
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

// https://www.khronos.org/opengl/wiki/Shader_Compilation

namespace SE {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		else if (type == "fragment")
		{
			return GL_FRAGMENT_SHADER;
		}
		SE_CORE_ASSERT(false, "Error type={0}", type);
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		SE_PROFILE_FUNCTION();

		std::string shaderSrc = ReadFile(filepath);
		auto shaderMaps = PreProcess(shaderSrc);
		Compile(shaderMaps);
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.find_last_of(".");
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
		: m_Name(name)
	{
		SE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSource;
		sources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		SE_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		SE_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			SE_CORE_ERROR("Could not find path: {0}", filepath);
		}
		return result;
	}
	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& shaderSrc)
	{
		SE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = shaderSrc.find(typeToken, 0);
		while (pos != shaderSrc.npos)
		{
			size_t eol = shaderSrc.find_first_of("\r\n", pos);
			SE_CORE_ASSERT(eol!=std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = shaderSrc.substr(begin, eol - begin);
			size_t nextLinePos = shaderSrc.find_first_not_of("\r\n", eol);
			pos = shaderSrc.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = shaderSrc.substr(nextLinePos, pos - ((nextLinePos == std::string::npos) ? shaderSrc.size()-1 : nextLinePos));
		}
		return shaderSources;
	}
	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaders)
	{
		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();
		SE_CORE_ASSERT(shaders.size() <= 2, "We only support 2 shaders for now!");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIdx = 0;
		//glShaderIDs.reserve(shaders.size());

		for (auto& kv : shaders)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			// Read our shaders into the appropriate buffers
			// std::string& vertexSource = // Get source code for vertex shader.
			// std::string& fragmentSource = // Get source code for fragment shader.

			// Create an empty vertex shader handle
			GLuint shader = glCreateShader(type);

			const GLchar* sourceCtr = source.c_str();
			glShaderSource(shader, 1, &sourceCtr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				SE_CORE_ERROR("{0}", infoLog.data());
				SE_CORE_ASSERT(false, "Shader compilation failure!");
				// In this simple program, we'll just leave
				return;
			}
			// Attach our shaders to our program
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIdx++] = shader;
		}
		
		
		m_RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto& shaderid : glShaderIDs)
				glDeleteShader(shaderid);

			// Use the infoLog as you see fit.
			SE_CORE_ERROR("{0}", infoLog.data());
			SE_CORE_ASSERT(false, "OpenGLShader link failure!");
			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (auto& shaderid : glShaderIDs)
			glDetachShader(program, shaderid);
	}

	void OpenGLShader::Bind() const
	{
		SE_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}
	void OpenGLShader::Unbind() const 
	{
		SE_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int val)
	{
		SE_PROFILE_FUNCTION();

		UploadUniformInt(name, val);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		SE_PROFILE_FUNCTION();

		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& mat)
	{
		SE_PROFILE_FUNCTION();

		UploadUniformMat4(name, mat);
	}
	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& val)
	{
		SE_PROFILE_FUNCTION();

		UploadUniformFloat3(name, val);
	}
	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& val)
	{
		SE_PROFILE_FUNCTION();

		UploadUniformFloat4(name, val);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int val)
	{
		SE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, val);

	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		SE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float val) {
		SE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, val);
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& vec)
	{
		SE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, vec.x, vec.y);
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vec)
	{
		SE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, vec.x, vec.y, vec.z);
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vec)
	{
		SE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
	}
	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		SE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		SE_PROFILE_FUNCTION();

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}