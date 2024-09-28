#include <SimpleEngine.h>

#include "imgui/imgui.h"


class ExampleLayer : public SE::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
		, m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
		, m_CameraPosition(0.0f, 0.0f, 0.0f)
		
	{
		m_VertexArray.reset(SE::VertexArray::Create());


		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.9f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.2f, 0.9f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.0f, 1.0f,
		};

		std::shared_ptr<SE::VertexBuffer> vb;
		vb.reset(SE::VertexBuffer::Create(vertices, sizeof(vertices)));

		vb->SetLayout({
			{SE::ShaderDataType::Float3, "a_Position"},
			{SE::ShaderDataType::Float4, "a_Color"},
			});
		m_VertexArray->AddVertexBuffer(vb);

		std::shared_ptr<SE::IndexBuffer> ib;
		unsigned int indices[3] = { 0, 1, 2 };
		ib.reset(SE::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(ib);


		float squarevertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};

		m_SquareVA.reset(SE::VertexArray::Create());
		std::shared_ptr<SE::VertexBuffer> sqVb;
		sqVb.reset(SE::VertexBuffer::Create(squarevertices, sizeof(squarevertices)));

		sqVb->SetLayout({
			{SE::ShaderDataType::Float3, "a_Position"},
			});
		m_SquareVA->AddVertexBuffer(sqVb);

		unsigned int squareindices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<SE::IndexBuffer> sqIb;
		sqIb.reset(SE::IndexBuffer::Create(squareindices, sizeof(squareindices) / sizeof(uint32_t)));

		m_SquareVA->SetIndexBuffer(sqIb);

		std::string vertexSrc = R"(
		#version 330 core
		
		layout(location=0)	in vec3 a_Position;
		layout(location=1)	in vec4 a_Color;

		uniform mat4 u_ViewProjection;
		
		out vec3 v_Position;
		out vec4 v_Color;

		void main()
		{
			v_Position = a_Position;
			v_Color = a_Color;
			gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
		}

)";
		std::string fragmentSrc = R"(
		#version 330 core
		
		layout(location=0)	out vec4 color;



		in vec3 v_Position;
		in vec4 v_Color;

		void main()
		{
			color = v_Color;
		}

)";

		m_Shader.reset(new SE::Shader(vertexSrc, fragmentSrc));


		std::string blueVertexSrc = R"(
		#version 330 core
		
		layout(location=0)	in vec3 a_Position;

		uniform mat4 u_ViewProjection;

		
		out vec3 v_Position;

		void main()
		{
			v_Position = a_Position;
			gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
		}

)";
		std::string blueFragmentSrc = R"(
		#version 330 core
		
		layout(location=0)	out vec4 color;

		in vec3 v_Position;

		void main()
		{
			color = vec4(0.2f, 0.2f, 0.8f, 1.0f);
		}

)";

		m_BlueShader.reset(new SE::Shader(blueVertexSrc, blueFragmentSrc));
	}

	void OnUpdate() override
	{

		if (SE::Input::IsKeyPressed(SE_KEY_LEFT))
		{
			m_CameraPosition.x += m_CameraMoveSpeed;
		}
		else if (SE::Input::IsKeyPressed(SE_KEY_RIGHT))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed;
		}

		if (SE::Input::IsKeyPressed(SE_KEY_DOWN))
		{
			m_CameraPosition.y += m_CameraMoveSpeed;
		}
		else if (SE::Input::IsKeyPressed(SE_KEY_UP))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed;
		}

		if (SE::Input::IsKeyPressed(SE_KEY_A))
		{
			m_CameraRotation -= m_CameraRotationSpeed;
		}
		else if (SE::Input::IsKeyPressed(SE_KEY_D))
		{
			m_CameraRotation += m_CameraRotationSpeed;
		}



		SE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		SE::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		SE::Renderer::BeginScene(m_Camera);
		//m_SquareVA->Bind();
		/*m_BlueShader->Bind();
		m_BlueShader->UploadUniformMat4("u_ViewProjection", m_Camera.GetViewProjectionMatrix());*/

		SE::Renderer::Submit(m_BlueShader, m_SquareVA);
		//glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

		//m_VertexArray->Bind();
		/*m_Shader->Bind();
		m_Shader->UploadUniformMat4("u_ViewProjection", m_Camera.GetViewProjectionMatrix());*/
		SE::Renderer::Submit(m_Shader, m_VertexArray);
		SE::Renderer::EndScene();
	}

  	void OnEvent(SE::Event& event) override
	{
		/*SE::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<SE::KeyPressedEvent>(BIND_EVENT_FN_X(ExampleLayer::OnKeyPressedEvent));*/
	}

	bool OnKeyPressedEvent(SE::KeyPressedEvent& event)
	{
		

		return false;
	}

	virtual void OnImGuiRender() override
	{
		/*ImGui::Begin("Test:");
		ImGui::Text("Hello World!");
		ImGui::End();*/
	}

private:
	std::shared_ptr<SE::Shader> m_Shader;
	std::shared_ptr<SE::VertexArray> m_VertexArray;

	std::shared_ptr<SE::Shader> m_BlueShader;
	std::shared_ptr<SE::VertexArray> m_SquareVA;

	SE::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 0.1f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 0.1f;
};

class Sandbox : public SE::Application
{
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}
};

SE::Application* SE::CreateApplication()
{
	return new Sandbox();
}