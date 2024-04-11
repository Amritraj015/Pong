cmake_minimum_required(VERSION 3.27)
project(Engine)
set(CMAKE_CXX_STANDARD 23)

include_directories(include)

file(GLOB_RECURSE LIBRARY_FILES "include/*.h" "src/*.cpp")
add_library(${PROJECT_NAME} ${LIBRARY_FILES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)