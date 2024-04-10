local proj_name = "lab"

workspace(proj_name)

architecture("x64")
configurations({ "debug", "release" })

local outputdir = "%{cfg.buildcfg}/%{cfg.architecture}"

project(proj_name)

location("src/")

kind("ConsoleApp")
language("C++")

pchheader("pch.hpp")
pchsource("src/main/pch.cpp")

targetdir("output/bin/" .. outputdir)
objdir("output/bin-int/" .. outputdir)

files({ "src/**.hpp", "src/**.cpp" })

cppdialect("C++2a")
staticruntime("On")
warnings("Extra")

exceptionhandling("Off")
rtti("Off")

characterset("MBCS")
includedirs({ "src/", "src/main" })

links({
	"luajit-5.1",
	"fmt",
})

filter("configurations:debug")
defines("DEBUG")
symbols("On")

filter("configurations:release")
defines("RELEASE")
optimize("Speed")
