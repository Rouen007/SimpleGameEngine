#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "SimpleEngine/Events/Event.h"
#include "SimpleEngine/Events/ApplicationEvent.h"
#include "SimpleEngine/ImGui/ImGuiLayer.h"
#include "SimpleEngine/Core/Timestep.h"




namespace SE
{
	class Application
	{
	public:
		Application(); 
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		bool m_Minimize = false;


		static Application* s_Instance;
	};

	Application* CreateApplication();
}


