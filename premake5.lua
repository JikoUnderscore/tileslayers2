workspace "tileslayers2"
	architecture "x64"

    configurations { 
        "Debug", 
        "Release" 
    }

    startproject "tileslayers2"


project "tileslayers2"
    location "tileslayers2"
    kind "ConsoleApp"
    language "C++"

    warnings "High"

    targetdir ("%{wks.location}/target/dis.%{prj.name}/%{cfg.architecture}/%{cfg.buildcfg}")
    objdir ("%{wks.location}/target/build.%{prj.name}/%{cfg.architecture}/%{cfg.buildcfg}")

    files {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.ixx",
        "%{prj.name}/src/**.cppm",
        "premake5.lua",
		-- "%{wks.location}/target/**.*"
    }



    filter { "system:windows" }
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"
		conformancemode "Yes"


        filter { "system:windows", "configurations:Debug" }
            runtime "Debug"
            symbols "On"
            links { "SDL2-staticd", "Setupapi", "winmm", "imm32", "version", "fmtd" }
            libdirs { "$(VCPKG_ROOT)/installed/x64-windows-static/debug/lib/manual-link", "$(VCPKG_ROOT)/installed/x64-windows-static/debug/lib" }
            buildoptions "/MTd /interface /w14242 /w14254 /w14263 /w14265 /w14287 /we4289 /w14296 /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14619 /w14640 /w14826 /w14905 /w14906 /w14928 /permissive- /W4"
            

        filter { "system:windows", "configurations:Release" }
			exceptionhandling ("Off")
            runtime "Release"
			flags { "LinkTimeOptimization" }
            links { "SDL2-static", "Setupapi", "winmm", "imm32", "version", "fmt" }
            libdirs { "$(VCPKG_ROOT)/installed/x64-windows-static/lib/manual-link", "$(VCPKG_ROOT)/installed/x64-windows-static/lib" }
            optimize "Speed"
            buildoptions "/MT /interface /w14242 /w14254 /w14263 /w14265 /w14287 /we4289 /w14296 /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14619 /w14640 /w14826 /w14905 /w14906 /w14928 /permissive- /W4"
