#pragma once 
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>
#include "SimpleEngine/Renderer/Shader.h"

typedef unsigned int GLenum; // todo remove
namespace SE
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filepath);
		virtual ~OpenGLShader();
		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformInt(const std::string& name, int val);
		
		void UploadUniformFloat(const std::string& name, float val);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& vec);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& vec);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& vec);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& shadeSrc);
		void Compile(std::unordered_map<GLenum, std::string>& shaders);
	private:
		uint32_t m_RendererID;
	};
}
