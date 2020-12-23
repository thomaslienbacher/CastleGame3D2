cmake_minimum_required(VERSION 3.17)
project(CastleGame3D2)

set(CMAKE_CXX_STANDARD 20)

if (${CMAKE_BUILD_TYPE} STREQUAL Release)
    add_definitions("-DRELEASE_BUILD")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mwindows -Wall -Wextra")
elseif (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    # set(CMAKE_VERBOSE_MAKEFILE on)
    add_definitions("-DDEBUG_BUILD")
endif ()

set(SOURCES src/main.cpp src/window.cpp src/window.hpp src/master.hpp src/mesh.cpp src/mesh.hpp src/shader.cpp src/shader.hpp src/utils.cpp src/utils.hpp src/texture.cpp src/texture.hpp src/camera.cpp src/camera.hpp src/debug.cpp src/debug.hpp src/geometry.cpp src/geometry.hpp src/game.cpp src/game.hpp)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(glad)
add_subdirectory(reactphysics3d)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} reactphysics3d)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/incslude/)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glm/)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/reactphysics3d/include/)
