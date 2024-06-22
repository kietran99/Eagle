project "Eagle"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"

    files
    {
        "Source/**.h",
        "Source/**.cpp",
    }

    includedirs
    {
        "Source",
    }

    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"
        defines {}

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
        symbols "Off"
