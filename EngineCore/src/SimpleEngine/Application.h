#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "SimpleEngine/Events/ApplicationEvent.h"
#include "SimpleEngine/ImGui/ImGuiLayer.h"
#include "SimpleEngine/Renderer/Shader.h"
#include "SimpleEngine/Renderer/Buffer.h"
#include "SimpleEngine/Renderer/VertexArray.h"


namespace SE
{
	class SENGINE_API Application
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
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;


		static Application* s_Instance;
	};

	Application* CreateApplication();
}


