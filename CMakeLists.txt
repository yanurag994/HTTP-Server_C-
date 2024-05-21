cmake_minimum_required(VERSION 3.13)
project(http-server-starter-cpp)
set(CMAKE_CXX_STANDARD 20) # Enable the C++20 standard
set (CMAKE_CXX_STANDARD_LIBRARIES "-lpthread")
set(SOURCE_FILES src/server.cpp src/request.hpp src/response.hpp)

add_executable(server ${SOURCE_FILES})
