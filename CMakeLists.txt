cmake_minimum_required(VERSION 3.22)
project(WheelPFC)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(wheel_core)
add_subdirectory(wheel_io)

add_executable(WheelPFC main.cpp)
target_link_libraries(WheelPFC PUBLIC libPFCcore)
target_link_libraries(WheelPFC PUBLIC libPFCio)