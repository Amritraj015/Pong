cmake_minimum_required(VERSION 3.27)
project(Pong)
set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} Engine)
