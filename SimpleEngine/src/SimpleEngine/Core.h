#pragma once


#ifdef SE_PLATFORM_WINDOWS
	#ifdef SE_BUILD_DLL
		#define SIMPLEENGINE_API __declspec(dllexport)
	#else 
		#define SIMPLEENGINE_API __declspec(dllimport)
	#endif
#else
	#error SIMPLE ENGINE ONLY SUPPORT WINDOWS
#endif

#define BIT(x) (1 << x)