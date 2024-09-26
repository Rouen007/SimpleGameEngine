#include "sepch.h"
#include "Application.h"
#include "SimpleEngine/Log.h"

#include <glad/glad.h>
#include "Input.h"
// #include "SimpleEngine/Renderer/Buffer.h"
#include "SimpleEngine/Renderer/Renderer.h"

namespace SE
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

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

	Application::Application()
		: m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
	{
		SE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_ImGuiLayer =  new ImGuiLayer();

		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());
		

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.9f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.2f, 0.9f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.0f, 1.0f,
		};

		std::shared_ptr<VertexBuffer> vb;
		vb.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		vb->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			});
		m_VertexArray->AddVertexBuffer(vb);
		
		std::shared_ptr<IndexBuffer> ib;
		unsigned int indices[3] = { 0, 1, 2 };
		ib.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(ib);


		float squarevertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f, 
			 0.5f, -0.5f, 0.0f, 
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};

		m_SquareVA.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> sqVb;
		sqVb.reset(VertexBuffer::Create(squarevertices, sizeof(squarevertices)));

		sqVb->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			});
		m_SquareVA->AddVertexBuffer(sqVb);

		unsigned int squareindices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr< IndexBuffer> sqIb;
		sqIb.reset(IndexBuffer::Create(squareindices, sizeof(squareindices) / sizeof(uint32_t)));

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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));


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

		m_BlueShader.reset(new Shader(blueVertexSrc, blueFragmentSrc));

	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		SE_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			--it;
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		SE_CLIENT_TRACE(e.ToString());
		if (e.IsInCategory(EventCategoryApplication))
		{
			
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			SE_CLIENT_TRACE(e);
		}
		
		while (m_Running)
		{
			//glClearColor(0.1f, 0.1f, 0.1f, 1);
			//glClear(GL_COLOR_BUFFER_BIT);

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();

			m_Camera.SetPosition({ 0.f, 0.f, 0.0f });
			m_Camera.SetRotation(45.f);

			Renderer::BeginScene(m_Camera);
			//m_SquareVA->Bind();
			/*m_BlueShader->Bind();
			m_BlueShader->UploadUniformMat4("u_ViewProjection", m_Camera.GetViewProjectionMatrix());*/

			Renderer::Submit(m_BlueShader, m_SquareVA);
			//glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			//m_VertexArray->Bind();
			/*m_Shader->Bind();
			m_Shader->UploadUniformMat4("u_ViewProjection", m_Camera.GetViewProjectionMatrix());*/
			Renderer::Submit(m_Shader, m_VertexArray);

			Renderer::EndScene();

			//glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);


			for (auto layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (auto layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();
			/*auto [x, y] = Input::GetMousePosition();
			SE_CORE_TRACE("{0}, {1}", x, y);*/
			m_Window->OnUpdate();
		}
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay); 
		overlay->OnAttach();

	}
}