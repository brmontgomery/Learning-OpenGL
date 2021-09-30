workspace "The_OpenGL_Project"
	architecture "x86_64"
	startproject "The_OpenGL_Project"

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
IncludeDir["assimp"] = "%{wks.location}/vendor/assimp/assimp/include"
IncludeDir["GLFW"] = "%{wks.location}/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/vendor/Glad/include"
IncludeDir["glm"] = "%{wks.location}/vendor/glm"
IncludeDir["stb"] = "%{wks.location}/vendor/stb-master"

group "Dependencies"
	include "vendor/GLFW"
	include "vendor/Glad"

group ""

project "The_OpenGL_Project"
	location "The_OpenGL_Project"
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
		"vendor/assimp/assimp/include/assimp/**.h",
		"vendor/assimp/assimp/include/assimp/**.hpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/KHR/**.h",
		"vendor/spdlog/include/**.h",
		"vendor/spdlog/include/**.cpp",
		"vendor/stb-master/stb-master/stb_image.h",
		"%{prj.name}/assets/**",

		--"assets/shaders/**.spv",
		--"assets/models/**.obj",
		--"assets/textures/**.jpg"
	}

	removefiles { "OldShaders.h", "OldModels.h" }

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"%{prj.name}/src",
		"vendor/spdlog/include",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.stb}",
	}

	libdirs { 
		"vendor/glfw/bin/Debug-windows-x86_64/glfw",
		"vendor/Glad/bin/Debug-windows-x86_64/Glad",
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"assimp-vc140-mt.lib",
		"opengl32.lib",
		"IrrXML.lib",
	}

	flags { "NoPCH" }

	--resources
	filter "files:**.png"
		buildaction "Embed"
	filter "files:**.jpg"
		buildaction "Embed"
	filter "files:**.gltf"
		buildaction "Embed"
	filter "files:**.fbx"
		buildaction "Embed"
	filter "files:**.vert"
		buildaction "Embed"
	filter "files:**.frag"
		buildaction "Embed"

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

		defines "OPENGLPROJECT_DEBUG"
		runtime "Debug"
		symbols "on"

		libdirs {
			"vendor/assimp/assimp/build/code/Debug",
			"vendor/assimp/assimp/build/contrib/irrXML/Debug",
			"vendor/assimp/assimp/build/contrib/zlib/Debug"
		}

		links
		{
			"zlibd.lib"
		}

		postbuildcommands {
			"{COPY} %{wks.location}vendor/assimp/assimp/build/code/Debug/assimp-vc140-mt.dll %{cfg.targetdir}"
		}

	filter "configurations:Release"

		defines "OPENGLPROJECT_RELEASE"
		runtime "Release"
		optimize "on"

		libdirs {
			"vendor/assimp/assimp/build/code/Release",
			"vendor/assimp/assimp/build/contrib/irrXML/Release",
			"vendor/assimp/assimp/build/contrib/zlib/Release"
		}

		links
		{
			"zlib.lib"
		}

		postbuildcommands {
			"{COPY} %{wks.location}vendor/assimp/assimp/build/code/Release/assimp-vc140-mt.dll %{cfg.targetdir}"
		}

	filter "configurations:Dist"

		defines "OPENGLPROJECT_DIST"
		runtime "Release"
		optimize "on"