#include "sepch.h"
#include "OpenGLVertexArray.h"
#include <GLAD/glad.h>



namespace SE
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:	return GL_FLOAT;

			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:		return GL_FLOAT;

			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:		return GL_INT;

			case ShaderDataType::Bool:		return GL_BOOL;
		}
		SE_CORE_ASSERT(false, "Unknown Type declared!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
		
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
	
	void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer>& vertexBuffer)
	{
		SE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& ele : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, ele.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(ele.Type),
				ele.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)ele.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);

	}

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}