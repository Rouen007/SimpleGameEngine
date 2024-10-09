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
	float squarevertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f, 
		 0.5f, -0.5f, 0.0f, 
		 0.5f,  0.5f, 0.0f, 
		-0.5f,  0.5f, 0.0f, 
	};

	m_SquareVA = SE::VertexArray::Create();
	SE::Ref<SE::VertexBuffer> sqVb;
	sqVb.reset(SE::VertexBuffer::Create(squarevertices, sizeof(squarevertices)));

	sqVb->SetLayout({
		{SE::ShaderDataType::Float3, "a_Position"},
		});
	m_SquareVA->AddVertexBuffer(sqVb);

	unsigned int squareindices[6] = { 0, 1, 2, 2, 3, 0 };
	SE::Ref<SE::IndexBuffer> sqIb;
	sqIb.reset(SE::IndexBuffer::Create(squareindices, sizeof(squareindices) / sizeof(uint32_t)));

	m_SquareVA->SetIndexBuffer(sqIb);

	m_FlatColorShader = SE::Shader::Create("assets/shaders/FlatColor.glsl");
	
}
void SandBox2D::OnDetach() 
{

}
void SandBox2D::OnUpdate(SE::Timestep ts) 
{
	m_CameraController.OnUpdate(ts);

	SE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	SE::RenderCommand::Clear();



	SE::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<SE::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<SE::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	

	SE::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	SE::Renderer::EndScene();

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
