cmake_minimum_required(VERSION 3.27)
project(Engine)
set(CMAKE_CXX_STANDARD 23)

include_directories(include)

file(GLOB_RECURSE LIBRARY_FILES "include/*.h" "src/*.h" "src/*.cpp")
add_library(${PROJECT_NAME} ${LIBRARY_FILES})