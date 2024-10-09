#pragma once
#include <memory>

#ifdef SE_PLATFORM_WINDOWS
#if SE_DYNAMIC_LINK
	#ifdef SE_BUILD_DLL
		#define SENGINE_API __declspec(dllexport)
	#else 
		#define SENGINE_API __declspec(dllimport)
	#endif
#else
	#define SENGINE_API 
#endif
#else
	#error SIMPLE ENGINE ONLY SUPPORT WINDOWS
#endif

#ifdef SE_DEBUG
	#define SE_ENABLE_ASSERTS
#endif

#ifdef SE_ENABLE_ASSERTS
	#define SE_ASSERT(x, ...) {if (!(x)) {SE_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define SE_CORE_ASSERT(x, ...) {if (!(x)) {SE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define SE_ASSERT(x, ...)
	#define SE_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)

#define BIND_EVENT_FN_X(x) std::bind(&x, this, std::placeholders::_1)

namespace SE
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}

