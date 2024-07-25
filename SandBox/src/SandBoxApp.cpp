#include <SimpleEngine.h>

class Sandbox : public SE::Application
{
public:
	Sandbox() {

	}
	~Sandbox() {}
};

SE::Application* SE::CreateApplication()
{
	return new Sandbox();
}