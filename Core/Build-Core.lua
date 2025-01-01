project "Eagle"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    staticruntime "Off"
    exceptionhandling "Off"

    files
    {
        "Public/**.h",
    }

    filter "system:windows"
        files
        {
            "Source/Windows/**.h",
            "Source/Windows/**.cpp",
        }

    filter {}
    
    includedirs
    {
        "Public",
        "Source",
    }

    filter "system:windows"
        pchheader "Pch.h"
        pchsource "Source/Windows/Pch.cpp"

    filter {}  

    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"
        defines { "NOMINMAX" }

    filter {}

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
