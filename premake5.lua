workspace "The OpenGL Project"
	architecture "x86_64"
	startproject "ErosionSim"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	disablewarnings 
	{
		"4201", "4100", "4189"
	}

	toolset "v142"

	buildoptions
	{
		"-Zi",
		--"-WX",
		"-W4",
		"-FC",
	}


outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Assimp"] = "vendor/assimp/assimp/include"
IncludeDir["GLFW"] = "vendor/GLFW/include"
IncludeDir["Glad"] = "vendor/Glad/include"
IncludeDir["glm"] = "vendor/glm"
IncludeDir["stb"] = "vendor/stb-master"

Library = {}


group "Dependencies"
	include "vendor/GLFW"
	include "vendor/Glad"
	include "vendor/assimp/assimp"

group ""

project "The OpenGL Project"
	location "The OpenGL Project"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/KHR/**.h",
		"vendor/spdlog/include/**.h",
		"vendor/spdlog/include/**.cpp",
		"vendor/stb-master/stb-master/stb_image.h",

		--"assets/shaders/**.spv",
		--"assets/models/**.obj",
		--"assets/textures/**.jpg"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"%{prj.name}/EngineSrc",
		"vendor/spdlog/include",
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.stb}"
	}

	libdirs { 
		"vendor/glfw/bin/Debug-windows-x86_64/glfw",
		"vendor/Glad/bin/Debug-windows-x86_64/Glad",
		"vendor/assimp/assimp/bin/Debug-windows-x86_64/assimp"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"opengl32.lib",
		"assimp"
	}

	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"UNICODE",
		}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"

		defines "OpenGLProject_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"

		defines "OpenGLProject_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"

		defines "OpenGLProject_DIST"
		runtime "Release"
		optimize "on"