#pragma once

#ifdef SE_PLATFORM_WINDOWS

extern SE::Application* SE::CreateApplication();

int main(int argc, char** argv) {
	printf("Simple Engine 444\n");
	auto app = SE::CreateApplication();
	app->Run();
	delete app;
}
#endif
