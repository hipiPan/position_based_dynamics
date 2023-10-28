cmake_minimum_required(VERSION 3.13)
project(pbd)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SOURCE_FILES "src/*.h" "src/*.cpp")
add_executable(pbd ${SOURCE_FILES})