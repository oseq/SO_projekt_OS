cmake_minimum_required(VERSION 3.7)
project(SO_projekt_OS)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lcurses")
set(SOURCE_FILES main.cpp)
add_executable(SO_projekt_OS ${SOURCE_FILES})