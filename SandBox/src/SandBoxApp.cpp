#include <SimpleEngine.h>

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
	}

	void OnEvent(SE::Event& e) override
	{
		SE_CORE_TRACE("{0}", e.ToString());
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