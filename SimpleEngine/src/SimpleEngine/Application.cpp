#include "Application.h"
#include "SimpleEngine/Events/ApplicationEvent.h"
#include "SimpleEngine/Log.h"
#include <iostream>

namespace SE
{
	Application::Application()
	{

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
		while (true)
		{

		}
	}
}