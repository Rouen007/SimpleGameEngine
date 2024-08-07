#pragma once


#ifdef SE_PLATFORM_WINDOWS
	#ifdef SE_BUILD_DLL
		#define SENGINE_API __declspec(dllexport)
	#else 
		#define SENGINE_API __declspec(dllimport)
	#endif
#else
	#error SIMPLE ENGINE ONLY SUPPORT WINDOWS
#endif

#ifdef SE_ENABLE_ASSERTS
	#define SE_ASSERT(x, ...) {if (!(x)) {SE_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak;}}
	#define SE_CORE_ASSERT(x, ...) {if (!(x)) {SE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak;}}
#else
	#define SE_ASSERT(x, ...)
	#define SE_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

