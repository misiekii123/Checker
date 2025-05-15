cmake_minimum_required(VERSION 3.10)

project(Checker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) 
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) 

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

add_library(board src/board/board.cpp src/board/board.h)
add_library(game src/game/game.cpp src/game/game.h)
add_library(pawn src/pawn/pawn.cpp src/pawn/pawn.h)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(board PRIVATE src)
target_include_directories(game PRIVATE src)
target_include_directories(pawn PRIVATE src)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib board game pawn)