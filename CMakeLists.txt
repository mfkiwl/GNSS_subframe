cmake_minimum_required(VERSION 3.16)
project(anontest)

set(CMAKE_CXX_STANDARD 11)


find_package(Boost REQUIRED)
add_executable(anontest main.cpp)
