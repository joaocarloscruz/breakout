cmake_minimum_required(VERSION 3.24)
project(BreakoutGame VERSION 1.1.0 LANGUAGES CXX)

include(FetchContent)
include(CTest)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")
set(SFML_BUILD_AUDIO OFF CACHE BOOL "Build SFML audio")
set(SFML_BUILD_NETWORK OFF CACHE BOOL "Build SFML network")
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "Build SFML examples")
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "Build SFML tests")
set(SFML_USE_STATIC_STD_LIBS ON CACHE BOOL "Use static compiler runtime")
if(MSVC AND SFML_USE_STATIC_STD_LIBS)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(SFML)

add_library(breakout_core
    src/core/Ball.cpp src/core/Brick.cpp src/core/BrickManager.cpp
    src/core/Paddle.cpp src/core/Level.cpp src/core/GameSession.cpp)
target_compile_features(breakout_core PUBLIC cxx_std_20)
target_include_directories(breakout_core PUBLIC src)
target_link_libraries(breakout_core PUBLIC SFML::Graphics)

add_library(breakout_ui src/ui/MainMenu.cpp src/core/GameManager.cpp src/core/Status.cpp)
target_link_libraries(breakout_ui PUBLIC breakout_core)
add_executable(breakout src/main.cpp)
target_link_libraries(breakout PRIVATE breakout_ui)
if(WIN32)
    set_target_properties(breakout PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

function(breakout_compiler_options target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
        if(MINGW)
            target_link_options(${target} PRIVATE -static -static-libgcc -static-libstdc++)
        endif()
    endif()
endfunction()
breakout_compiler_options(breakout_core)
breakout_compiler_options(breakout_ui)
breakout_compiler_options(breakout)

add_custom_command(TARGET breakout POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:breakout>/assets")
if(BUILD_TESTING)
    add_executable(breakout_tests tests/GameSessionTests.cpp)
    target_link_libraries(breakout_tests PRIVATE breakout_core)
    breakout_compiler_options(breakout_tests)
    add_test(NAME gameplay COMMAND breakout_tests)
    add_test(NAME startup COMMAND breakout --check)
    add_executable(breakout_render_test tests/RenderSmoke.cpp)
    target_link_libraries(breakout_render_test PRIVATE breakout_ui)
    breakout_compiler_options(breakout_render_test)
    add_test(NAME rendering COMMAND breakout_render_test
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/font/sansation.ttf" "${CMAKE_CURRENT_BINARY_DIR}/screenshots")
endif()

install(TARGETS breakout RUNTIME DESTINATION . COMPONENT Runtime)
install(DIRECTORY assets DESTINATION . COMPONENT Runtime)
install(FILES LICENSE README.md DESTINATION . COMPONENT Runtime)
install(DIRECTORY licenses DESTINATION . COMPONENT Runtime)
install(FILES THIRD_PARTY_NOTICES.md DESTINATION . COMPONENT Runtime)
