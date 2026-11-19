workspace "sadlib"
    configurations { "debug", "release" }
    platforms { "win-x86", "win-x64" }

    targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}/"
    objdir"obj/%{cfg.platform}/%{cfg.buildcfg}/"

    defaultplatform "win-x64"
    startproject "test"

    filter "platforms:*x86"
        architecture "x86"

    filter "platforms:*x64"
        architecture "x86_64"
    
    filter "action:vs*"
        buildoptions { "/utf-8" }
    
    filter ""

project "sadlib"
    kind "StaticLib"
    language "C"
    cdialect "C89"

    files {"src/**.c", "src/**.h", "include/**.h"}
    includedirs { "include" }
    
    filter "kind:SharedLib"
        defines { "SADLIB_BUILD_SHARED" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "test"
    kind "ConsoleApp"
    language "C"
    cdialect "C89"

    files {"test/**.c", "test/**.h"}
    includedirs { "include" }
    links { "sadlib" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"