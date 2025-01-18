workspace "Eagle-Test"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Test"

    filter "system:windows"
        buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
    include "Core/Build-Core.lua"
group ""

include "Test/Build-Test.lua"