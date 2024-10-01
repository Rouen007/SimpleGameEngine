#pragma once

#include "RendererAPI.h"

namespace SE
{
	/*enum class RendererAPI
	{
		None = 0,
		OpenGL = 1,
	};*/

	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RenderAPI->Init();
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RenderAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_RenderAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RenderAPI->DrawIndexed(vertexArray);
		}

		//inline static RendererAPI* GetRenderAPI() { return s_RenderAPI; }
	private:
		static RendererAPI* s_RenderAPI;
	}; 
}
