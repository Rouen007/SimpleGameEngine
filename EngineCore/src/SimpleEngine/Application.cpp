#include "sepch.h"
#include "Application.h"
#include "SimpleEngine/Log.h"

#include <glad/glad.h>
#include "Input.h"
// #include "SimpleEngine/Renderer/Buffer.h"
#include "SimpleEngine/Renderer/Renderer.h"
#include <GLFW/glfw3.h>


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
	{
		SE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_ImGuiLayer =  new ImGuiLayer();

		PushOverlay(m_ImGuiLayer);

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
			float time = (float) glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (auto layer : m_LayerStack)
			{
				layer->OnUpdate(timestep);
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