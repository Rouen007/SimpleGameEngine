#pragma once

#include "sepch.h"

#include "SimpleEngine/Core.h"
#include "SimpleEngine/Events/Event.h"

namespace SE
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width, Height;
		
		WindowProps(const std::string& title = "Simple Engine",
			unsigned int width = 1280, unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{

		}
	};

	// interface
	class SENGINE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		
		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());



	};
}