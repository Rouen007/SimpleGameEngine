#pragma once
#include "Core.h"

namespace SE
{
	class SIMPLEENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}


