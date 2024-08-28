#pragma once
#include "SimpleEngine/Window.h"
#include "SimpleEngine/Renderer/GraphicsContexture.h"
#include <GLFW/glfw3.h>

namespace SE
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() override { return m_Data.Width; }
		inline unsigned int GetHeight() override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override
		{
			m_Data.EventCallback = callback;
		}
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		inline virtual void* GetNativeWindow() const { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
			WindowData()
				: Title("")
				, Width(0)
				, Height(0)
				, VSync(false)
			{}
		};

		WindowData m_Data;

		GraphicsContext* m_Context;

	};
}