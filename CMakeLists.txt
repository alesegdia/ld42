cmake_minimum_required(VERSION 2.8.11)

project(ld42)

add_subdirectory(module/aether)

include("module/aether/cmake/Build.cmake")

file(GLOB_RECURSE GAMESOURCES "src/*.cpp" "src/*.h")
ADD_AETHER_TARGET(ld42 "${GAMESOURCES}")


