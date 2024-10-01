#include <SimpleEngine.h>

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"
#include <memory>


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

		SE::Ref<SE::VertexBuffer> vb;
		vb.reset(SE::VertexBuffer::Create(vertices, sizeof(vertices)));

		vb->SetLayout({
			{SE::ShaderDataType::Float3, "a_Position"},
			{SE::ShaderDataType::Float4, "a_Color"},
			});
		m_VertexArray->AddVertexBuffer(vb);

		SE::Ref<SE::IndexBuffer> ib;
		unsigned int indices[3] = { 0, 1, 2 };
		ib.reset(SE::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(ib);


		float squarevertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		m_SquareVA.reset(SE::VertexArray::Create());
		SE::Ref<SE::VertexBuffer> sqVb;
		sqVb.reset(SE::VertexBuffer::Create(squarevertices, sizeof(squarevertices)));

		sqVb->SetLayout({
			{SE::ShaderDataType::Float3, "a_Position"},
			{SE::ShaderDataType::Float2, "a_TexCoord"},
			});
		m_SquareVA->AddVertexBuffer(sqVb);

		unsigned int squareindices[6] = { 0, 1, 2, 2, 3, 0 };
		SE::Ref<SE::IndexBuffer> sqIb;
		sqIb.reset(SE::IndexBuffer::Create(squareindices, sizeof(squareindices) / sizeof(uint32_t)));

		m_SquareVA->SetIndexBuffer(sqIb);

		std::string vertexSrc = R"(
		#version 330 core
		
		layout(location=0)	in vec3 a_Position;
		layout(location=1)	in vec4 a_Color;

		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;
		
		out vec3 v_Position;
		out vec4 v_Color;

		void main()
		{
			v_Position = a_Position;
			v_Color = a_Color;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

		m_Shader.reset(SE::Shader::Create(vertexSrc, fragmentSrc));


		std::string flatColorVertexSrc = R"(
		#version 330 core
		
		layout(location=0)	in vec3 a_Position;

		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;

		
		out vec3 v_Position;

		void main()
		{
			v_Position = a_Position;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
		}

)";

		std::string flatColorFragmentSrc = R"(
		#version 330 core
		
		layout(location=0)	out vec4 color;

		in vec3 v_Position;
		uniform vec3 u_Color;

		void main()
		{
			color = vec4(u_Color, 1.0f);
		}

)";

		m_FlatColorShader.reset(SE::Shader::Create(flatColorVertexSrc, flatColorFragmentSrc));

		std::string textureVertexSrc = R"(
		#version 330 core
		
		layout(location=0)	in vec3 a_Position;
		layout(location=1)	in vec2 a_TexCoord;

		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;

		
		out vec3 v_Position;
		out vec2 v_TexCoord;

		void main()
		{
			v_Position = a_Position;
			v_TexCoord = a_TexCoord;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
		}

)";
		std::string textureFragmentSrc = R"(
		#version 330 core
		
		layout(location=0)	out vec4 color;

		in vec3 v_Position;
		in vec2 v_TexCoord;
		uniform vec3 u_Color;
		uniform sampler2D u_Texture;

		void main()
		{
			color = texture(u_Texture, v_TexCoord);
		}

)";
		

		m_Texture = SE::Texture2D::Create("assets/textures/qiya2.jfif");
		m_TextureShader.reset(SE::Shader::Create(textureVertexSrc, textureFragmentSrc));

		std::dynamic_pointer_cast<SE::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<SE::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0); // slot=0
	}

	void OnUpdate(SE::Timestep ts) override
	{
		// SE_CLIENT_INFO("Delta time : {0} ms", ts.GetMilliseconds());

		if (SE::Input::IsKeyPressed(SE_KEY_LEFT))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}
		else if (SE::Input::IsKeyPressed(SE_KEY_RIGHT))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}

		if (SE::Input::IsKeyPressed(SE_KEY_DOWN))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}
		else if (SE::Input::IsKeyPressed(SE_KEY_UP))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
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

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

		std::dynamic_pointer_cast<SE::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<SE::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; ++y)
			for (int x = 0; x < 20; ++x)
			{
				// std::dynamic_pointer_cast<SE::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", (x % 2 == 0) ? redColor : blueColor);
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				SE::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		//SE::Renderer::Submit(m_Shader, m_VertexArray);

		m_Texture->Bind();
		SE::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

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
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

private:
	SE::Ref<SE::Shader> m_Shader;
	SE::Ref<SE::VertexArray> m_VertexArray;
	SE::Ref<SE::Shader> m_FlatColorShader, m_TextureShader;
	SE::Ref<SE::VertexArray> m_SquareVA;

	SE::Ref<SE::Texture2D> m_Texture;

	SE::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 0.5f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 0.2f;
	glm::vec3 m_SquareColor{ 0.2f, 0.3f, 0.8f };

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