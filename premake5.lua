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
IncludeDir["GLAD"] = "EngineCore/third_parties/GLAD/include"
IncludeDir["ImGui"] = "EngineCore/third_parties/imgui"
IncludeDir["glm"] = "EngineCore/third_parties/glm"

include "EngineCore/third_parties/GLFW"
include "EngineCore/third_parties/GLAD"
include "EngineCore/third_parties/imgui"

project "EngineCore"
    location "EngineCore"
    kind "SharedLib"
    language "C++"
    staticruntime "Off"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "sepch.h"
    pchsource "EngineCore/src/sepch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
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
        "%{IncludeDir.glm}"

    }

    links
    {
        "GLFW",
        "GLAD",
        "opengl32.lib",
        "ImGui"
    }


    filter "system:windows"
        cppdialect "c++17"
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "SE_BUILD_DLL",
            "SE_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox\"")
        }

    filter "configurations:Debug"
        defines "SE_DEBUG"
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines "SE_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "SE_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "Off"

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
        "%{IncludeDir.glm}"
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
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines "SE_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "SE_DIST"
        runtime "Release"
        optimize "On"