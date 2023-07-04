-- premake5.lua
workspace "LightRay"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "LightRay"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "LightRay"