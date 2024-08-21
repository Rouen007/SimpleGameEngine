#include <SimpleEngine.h>

#include "imgui/imgui.h"


class ExampleLayer : public SE::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		SE_CLIENT_INFO("ExampleLayer::Update");
		if (SE::Input::IsKeyPressed(SE_KEY_TAB))
			SE_CLIENT_INFO("Tab key is pressed!");
	}

  	void OnEvent(SE::Event& e) override
	{
		SE_CORE_TRACE("{0}", e.ToString());
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test:");
		ImGui::Text("Hello World!");
		ImGui::End();
	}
};

class Sandbox : public SE::Application
{
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}
};

SE::Application* SE::CreateApplication()
{
	return new Sandbox();
}