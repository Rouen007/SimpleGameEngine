#pragma once
#include "Core.h"
#include "Events/Event.h"

namespace SE
{
	class SENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}


