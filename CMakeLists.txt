cmake_minimum_required(VERSION 3.13)
project(http-server-starter-cpp)
set(CMAKE_CXX_STANDARD 23) # Enable the C++20 standard

# Find packages
find_package(ZLIB REQUIRED)
add_library(backend src/backend.cpp src/backend.hpp)

add_executable(server src/server.cpp src/server.hpp)

include_directories("deps")
include_directories("utils")

if(NOTHREADING)
    add_compile_definitions(NOTHREADING)
    target_link_libraries(server backend ZLIB::ZLIB)
else()   
    find_package(Threads REQUIRED)
    target_link_libraries(server backend ZLIB::ZLIB Threads::Threads)
endif()