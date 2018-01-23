cmake_minimum_required(VERSION 3.9)
project(Simple_UDP)

set(CMAKE_CXX_STANDARD 11)

add_executable(Simple_UDP server.cpp)
add_executable(client client.cpp)