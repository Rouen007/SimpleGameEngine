#include "sepch.h"
#include "Application.h"
#include "SimpleEngine/Events/ApplicationEvent.h"
#include "SimpleEngine/Log.h"

#include <GLFW/glfw3.h>

namespace SE
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());;
	}

	Application::~Application()
	{

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
}