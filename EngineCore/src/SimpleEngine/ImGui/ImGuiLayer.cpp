#include "sepch.h"
#include "ImGuiLayer.h"
#include "Platform/OpenGL/imgui_render.h"
#include "GLFW/glfw3.h"
#include "SimpleEngine/Application.h"

namespace SE
{
	
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{

	}
	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
		
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f?(time - m_Time):(1.0f/60);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void ImGuiLayer::OnEvent(Event& event)
	{

	}

	void ImGuiLayer::OnAttach() 
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiBackendFlags_HasMouseCursors;     
		io.ConfigFlags |= ImGuiBackendFlags_HasSetMousePos;      

		ImGui_ImplOpenGL3_Init("#version 410");

	}
	void ImGuiLayer::OnDetach() {}
	
}