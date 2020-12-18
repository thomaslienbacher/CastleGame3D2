cmake_minimum_required(VERSION 3.17)
project(GameDataFormat)

set(CMAKE_CXX_STANDARD 20)

if (${CMAKE_BUILD_TYPE} STREQUAL Release)
    add_definitions("-DRELEASE_BUILD")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mwindows -Wall -Wextra")
elseif (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    # set(CMAKE_VERBOSE_MAKEFILE on)
    add_definitions("-DDEBUG_BUILD")
endif ()

set(SOURCES include/dataformat/geometry.hpp src/geometry.cpp)

add_library(${PROJECT_NAME} ${SOURCES})

include_directories(include/dataformat/)
