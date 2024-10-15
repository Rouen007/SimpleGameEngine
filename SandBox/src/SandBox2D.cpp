#include "SandBox2D.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

SandBox2D::SandBox2D()
	: Layer("SandBox2D")
	, m_CameraController(1280.f / 720.f)
{

}

void SandBox2D::OnAttach() 
{
	m_QiyaTexture = SE::Texture2D::Create("assets/textures/qiya4.png");
	
}
void SandBox2D::OnDetach() 
{

}
void SandBox2D::OnUpdate(SE::Timestep ts) 
{
	m_CameraController.OnUpdate(ts);

	SE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	SE::RenderCommand::Clear();

	SE::Renderer2D::BeginScene(m_CameraController.GetCamera());

	/*std::dynamic_pointer_cast<SE::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<SE::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);*/
	SE::Renderer2D::DrawQuad({ -1.f, 0.f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	SE::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });

	SE::Renderer2D::DrawQuad({ 0.2f, 0.5f, -0.1f}, { 1.8f, 3.2f }, m_QiyaTexture);

	// SE::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	SE::Renderer2D::EndScene();

}

void SandBox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void SandBox2D::OnEvent(SE::Event& event)
{
	m_CameraController.OnEvent(event);
}
