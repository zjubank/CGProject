cmake_minimum_required(VERSION 3.3)
project(CGProject)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main.cpp
    mtl.cpp
    mtl.hpp
    obj.cpp
    obj.hpp)

add_executable(CGProject ${SOURCE_FILES})