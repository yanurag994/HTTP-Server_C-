cmake_minimum_required(VERSION 3.13)
project(http-server-starter-cpp)
set(CMAKE_CXX_STANDARD 20) # Enable the C++20 standard
set (CMAKE_CXX_STANDARD_LIBRARIES "-lpthread")

# Find packages
find_package(ZLIB REQUIRED)
#add_library(backend src/backend.cpp src/backend.hpp)

add_executable(server src/server.cpp src/server.hpp src/backend.cpp src/backend.hpp src/compression.hpp src/request.hpp src/response.hpp)

#include_directories("deps")
#include_directories("utils")

#if(NOTHREADING)
 #   add_compile_definitions(NOTHREADING)
  #  target_link_libraries(server backend ZLIB::ZLIB)
#else()   
target_link_libraries(server ZLIB::ZLIB)
#endif()