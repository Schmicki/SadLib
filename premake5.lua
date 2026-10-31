workspace "sadlib"
    configurations { "Debug", "Release" }
    platforms { "Win32", "Win64" }

    targetdir "bin/%{cfg.buildcfg}/"
    objdir"obj/%{cfg.buildcfg}/"

    defaultplatform "Win64"
    startproject "Test"

    filter "platforms:*32"
        architecture "x86"

    filter "platforms:*64"
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