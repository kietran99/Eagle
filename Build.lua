workspace "Eagle-Sandbox"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "App"

    filter "system:windows"
        buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
    include "Core/Build-Core.lua"
group ""

include "App/Build-App.lua"