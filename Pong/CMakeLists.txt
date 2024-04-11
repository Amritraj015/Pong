cmake_minimum_required(VERSION 3.27)
project(Pong)
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE APPLICATION_FILES "src/*.h" "src/*.cpp")
add_executable(${PROJECT_NAME} ${APPLICATION_FILES})

target_link_libraries(${PROJECT_NAME} Engine)
target_include_directories(${PROJECT_NAME} PRIVATE Engine)
