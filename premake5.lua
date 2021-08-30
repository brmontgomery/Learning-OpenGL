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

VULKAN_SDK = os.getenv("VULKAN_SDK")

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["assimp"] = "%{wks.location}/vendor/assimp/assimp/include"
IncludeDir["GLFW"] = "%{wks.location}/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/vendor/Glad/include"
IncludeDir["glm"] = "%{wks.location}/vendor/glm"
IncludeDir["stb"] = "%{wks.location}/vendor/stb-master"
IncludeDir["shaderc"] = "%{wks.location}/vendor/shaderc"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/vendor/spirv_cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/vendor/VulkanSDK/Lib"
LibraryDir["VulkanSDK_DebugDLL"] = "%{wks.location}/vendor/VulkanSDK/Bin"
LibraryDir["VulkanSDK_DebugDLL_Post_Build"] = "%{wks.location}vendor/VulkanSDK/Bin"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"


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
		"vendor/shaderc/**",
		"vendor/spirv_cross/**",

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
		"%{prj.name}/src",
		"vendor/spdlog/include",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.shaderc}",
		"%{IncludeDir.SPIRV_Cross}",
		"%{IncludeDir.VulkanSDK}"
	}

	libdirs { 
		"vendor/glfw/bin/Debug-windows-x86_64/glfw",
		"vendor/Glad/bin/Debug-windows-x86_64/Glad",
		"%{LibraryDir.VulkanSDK}",
		"%{LibraryDir.VulkanSDK_Debug}",
		"%{LibraryDir.VulkanSDK_DebugDLL_Post_Build}"
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
			"zlibd.lib",
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

		postbuildcommands {
			"{COPY} %{LibraryDir.VulkanSDK_DebugDLL_Post_Build}/shaderc_sharedd.dll %{cfg.targetdir}",
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
			"zlib.lib",
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

		postbuildcommands {
			"{COPY} %{wks.location}vendor/assimp/assimp/build/code/Release/assimp-vc140-mt.dll %{cfg.targetdir}"
		}

	filter "configurations:Dist"

		defines "OPENGLPROJECT_DIST"
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}