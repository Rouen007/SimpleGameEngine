#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"

namespace SE
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation = 0.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation = 0.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor=10.f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor = 10.f, float rotation=0.0f);
		
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexxCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();
	private:
		static void StartNewBatch();
	};
}
