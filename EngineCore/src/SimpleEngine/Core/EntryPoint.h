#pragma once

#ifdef SE_PLATFORM_WINDOWS

extern SE::Application* SE::CreateApplication();

int main(int argc, char** argv) {
	SE::Log::Init();
	SE_CORE_ERROR("Initializing core log!");
	SE_CLIENT_ERROR("Initializing client={0}", 5);
	printf("Simple Engine 444\n");
	SE_PROFILE_BEGIN_SESSION("Startup", "SEProfile-Startup.json");
	auto app = SE::CreateApplication();
	SE_PROFILE_END_SESSION();
	
	SE_PROFILE_BEGIN_SESSION("Runtime", "SEProfile-Runtime.json");
	app->Run();
	SE_PROFILE_END_SESSION();
	
	SE_PROFILE_BEGIN_SESSION("Shutdown", "SEProfile-Shutdown.json");
	delete app;
	SE_PROFILE_END_SESSION();

}
#endif
