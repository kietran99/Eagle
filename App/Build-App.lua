project "App"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    staticruntime "Off"
    exceptionhandling "Off"

    files
    {
        "Source/**.h",
        "Source/**.cpp",
    }

    includedirs
    {
        "Source",
        "../Core/Public",
    }

    links
    {
        "Eagle",
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