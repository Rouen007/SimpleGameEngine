workspace "SimpleEngine"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug", 
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {} 
IncludeDir["GLFW"] = "EngineCore/third_parties/GLFW/include"
IncludeDir["GLAD"] = "EngineCore/third_parties/GLAD/include"
IncludeDir["ImGui"] = "EngineCore/third_parties/imgui"
IncludeDir["glm"] = "EngineCore/third_parties/glm"
IncludeDir["stb_image"] = "EngineCore/third_parties/stb_image"

include "EngineCore/third_parties/GLFW"
include "EngineCore/third_parties/GLAD"
include "EngineCore/third_parties/imgui"

project "EngineCore"
    location "EngineCore"
    kind "StaticLib"
    language "C++"
    cppdialect "c++17"
    staticruntime "on"

    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "sepch.h"
    pchsource "EngineCore/src/sepch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/third_parties/stb_image/**.h",
        "%{prj.name}/third_parties/stb_image/**.cpp",
        "%{prj.name}/third_parties/glm/glm/**.hpp",
        "%{prj.name}/third_parties/glm/glm/**.inl",
    }

    includedirs
    {
        "%{prj.name}/third_parties/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}"

    }

    links
    {
        "GLFW",
        "GLAD",
        "opengl32.lib",
        "ImGui"
    }


    filter "system:windows"
        systemversion "latest"

        defines
        {
            "SE_BUILD_DLL",
            "SE_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        defines "SE_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "SE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "SE_DIST"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "c++17"
    staticruntime "on"

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
        "EngineCore/src",
        "EngineCore/third_parties",
        "%{IncludeDir.glm}"
    }

    links
    {
        "EngineCore"
    }

    filter "system:windows"
        cppdialect "c++17"
        staticruntime "on"
        systemversion "latest"

        defines
        {
            "SE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "SE_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "SE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "SE_DIST"
        runtime "Release"
        optimize "on"