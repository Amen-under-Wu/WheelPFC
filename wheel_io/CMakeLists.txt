cmake_minimum_required(VERSION 3.22)
project(WheelPFC)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.c* include/*.hpp)
add_library(libPFCio STATIC ${srcs})
target_include_directories(libPFCio PUBLIC include)
target_link_libraries(libPFCio PUBLIC libPFCcore)

link_directories(${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(libPFCio PUBLIC ${PROJECT_SOURCE_DIR}/lib/glfw3.dll)

