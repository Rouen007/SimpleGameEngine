#pragma once

#ifdef SE_PLATFORM_WINDOWS

extern SE::Application* SE::CreateApplication();

int main(int argc, char** argv) {
	SE::Log::Init();
	SE_CORE_ERROR("Initializing core log!");
	SE_CLIENT_ERROR("Initializing client={0}", 5);
	printf("Simple Engine 444\n");
	auto app = SE::CreateApplication();
	app->Run();
	delete app;
}
#endif
