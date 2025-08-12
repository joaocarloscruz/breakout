include(FetchContent)

cmake_minimum_required(VERSION 3.16)
project(BreakoutGame LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 20)
set(FLAGS "-Wno-unused-command-line-argument")
set(CMAKE_C_FLAGS ${FLAGS})
set(CMAKE_CXX_FLAGS ${FLAGS})

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(SFML)

set(SOURCE_FILES
    "src/main.cpp"
    "src/ui/MainMenu.cpp"
    "src/ui/include/MainMenu.h"
    "src/core/GameManager.cpp"
    "src/core/include/GameManager.h"
    "src/core/Paddle.cpp"
    "src/core/include/Paddle.h"
    "src/core/Ball.cpp"
    "src/core/include/Ball.h"
    "src/core/Brick.cpp"
    "src/core/include/Brick.h"
    "src/core/BrickManager.cpp"
    "src/core/include/BrickManager.h"
)

if(WIN32)
    add_executable(breakout WIN32 ${SOURCE_FILES})
else()
    add_executable(breakout ${SOURCE_FILES})
endif()

target_link_libraries(breakout PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_features(breakout PRIVATE cxx_std_17)
target_include_directories(breakout PRIVATE src)

add_custom_target(copy_font
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/assets/font/sansation.ttf ${CMAKE_BINARY_DIR}
)
add_dependencies(breakout copy_font)

install(TARGETS breakout)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
