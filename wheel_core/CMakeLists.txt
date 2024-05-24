file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.hpp)
add_library(libPFCcore STATIC ${srcs})
target_include_directories(libPFCcore PUBLIC include)