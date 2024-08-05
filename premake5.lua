workspace "SimpleEngine"
    architecture "x64"

    configurations
    {
        "Debug", 
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {} 
IncludeDir["GLFW"] = "EngineCore/third_parties/GLFW/include"

include "EngineCore/third_parties/GLFW"

project "EngineCore"
    location "EngineCore"
    kind "SharedLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "sepch.h"
    pchsource "EngineCore/src/sepch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/third_parties/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}"
    }

    links
    {
        "GLFW",
        "opengl32.lib"
    }


    filter "system:windows"
        cppdialect "c++17"
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "SE_BUILD_DLL",
            "SE_PLATFORM_WINDOWS"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox\"")
        }

    filter "configurations:Debug"
        defines "SE_DEBUG"
        buildoptions "/MDd"
        symbols "On"
    
    filter "configurations:Release"
        defines "SE_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "SE_DIST"
        buildoptions "/MD"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "EngineCore/third_parties/spdlog/include",
        "EngineCore/src"
    }

    links
    {
        "EngineCore"
    }

    filter "system:windows"
        cppdialect "c++17"
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "SE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "SE_DEBUG"
        buildoptions "/MDd"
        symbols "On"
    
    filter "configurations:Release"
        defines "SE_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "SE_DIST"
        buildoptions "/MD"
        optimize "On"