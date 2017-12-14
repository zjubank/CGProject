cmake_minimum_required(VERSION 3.3)
project(CGProject)

## OpenGL
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})

## default
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(SOURCE_FILES
        main.cpp
        glm.cpp
        Mtl.cpp
        Mtl.h
        Obj.cpp
        Obj.h)

add_executable(CGProject ${SOURCE_FILES})

## add linked libraries
TARGET_LINK_LIBRARIES(CGProject ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
