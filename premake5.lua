workspace "conway"
	architecture "x86"
	configurations { "Debug", "Release" }
	
outdir = "%{cfg.system}_%{cfg.architecture}"

project "conway"
	location "conway"
	kind "ConsoleApp"
	language "C"

	targetdir ("bin/" .. outdir .. "/%{prj.name}")
	objdir ("obj/" .. outdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/external/libsdl/include"
	}

	libdirs {
		"%{prj.name}/external/libsdl/lib/x86"
	}

	links {
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_image.lib",
                "SDL2_ttf.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines "_CRT_SECURE_NO_WARNINGS"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"