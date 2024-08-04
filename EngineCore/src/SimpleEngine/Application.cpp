#include "sepch.h"
#include "Application.h"
#include "SimpleEngine/Log.h"

#include <GLFW/glfw3.h>

namespace SE
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		SE_CORE_TRACE("{0}", e);
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
			m_Window->OnUpdate();
		}
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}