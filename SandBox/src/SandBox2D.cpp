#include "SandBox2D.h"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>

template<typename Fn>
class Timer
{
public: 
	Timer(const char* name, Fn&& func)
		: m_Name(name)
		, m_Func(func)
		, m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
		{
			Stop();
		}
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;
		
		float duration = (end - start) * 0.001f;
		m_Func({m_Name, duration});
		// std::cout << m_Name << ": " << (end - start)*0.001f << " ms" << std::endl;
	}

private:
	const char* m_Name;
	Fn m_Func;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
};


#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult);}) 
SandBox2D::SandBox2D()
	: Layer("SandBox2D")
	, m_CameraController(1280.f / 720.f)
{

}

void SandBox2D::OnAttach() 
{
	SE_PROFILE_FUNCTION();

	m_QiyaTexture = SE::Texture2D::Create("assets/textures/qiya4.png");
	
}
void SandBox2D::OnDetach() 
{
	SE_PROFILE_FUNCTION();
}
void SandBox2D::OnUpdate(SE::Timestep ts) 
{
	SE_PROFILE_FUNCTION("SandBox2D::OnUpdate");

	SE::Renderer2D::ResetStats();
	//update
	{
		SE_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}
	{
		SE_PROFILE_SCOPE("Renderer::Prep");
		SE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		SE::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 20.f;
		SE_PROFILE_SCOPE("Renderer::Draw");
		
		SE::Renderer2D::BeginScene(m_CameraController.GetCamera());

		/*std::dynamic_pointer_cast<SE::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<SE::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);*/
		
		SE::Renderer2D::DrawQuad({ -1.f, 0.f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });

		SE::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });

		SE::Renderer2D::DrawQuad({ 0.2f, 0.5f, -0.1f }, { 1.8f, 3.2f }, m_QiyaTexture);
		SE::Renderer2D::DrawQuad({ 0.5f, -0.5f, 0.0f }, { 0.9f, 1.6f }, m_QiyaTexture, 20.f, rotation);

		// SE::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		SE::Renderer2D::EndScene();

		SE::Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 rb = { (x + 5.0f) / 10.f, 0.4f, (y + 5.0f) / 10.f, 1.0f };
				SE::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, rb);

			}

		}
		SE::Renderer2D::EndScene();
	}

}

void SandBox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	auto stats = SE::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls:  %d", stats.DrawCalls);
	ImGui::Text("Quad Count:  %d", stats.QuadCount);
	ImGui::Text("Vertices:  %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices:  %d", stats.GetTotalIndexxCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	for (auto& result: m_ProfileResults)
	{
		char label[50];
		strcpy(label, "  %.3fms ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}

	m_ProfileResults.clear();
	ImGui::End();
}

void SandBox2D::OnEvent(SE::Event& event)
{
	m_CameraController.OnEvent(event);
}
