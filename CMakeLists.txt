cmake_minimum_required(VERSION 3.5)
project(Checker)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES
    src/board/*.cpp
    src/game/*.cpp
    src/pawn/*.cpp
    src/ui/*.cpp
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        master
)

set(FETCHCONTENT_QUIET OFF)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

target_link_libraries(${PROJECT_NAME} raylib)

FetchContent_Declare(
    raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui.git
    GIT_TAG        3.5
)

FetchContent_MakeAvailable(raygui)

target_include_directories(${PROJECT_NAME} PRIVATE ${raygui_SOURCE_DIR}/src)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} winmm gdi32 opengl32)
endif()
