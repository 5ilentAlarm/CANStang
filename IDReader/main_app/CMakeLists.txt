cmake_minimum_required(VERSION 3.27)
project(CANU_I C)
set(CMAKE_C_STANDARD 99)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

set(includedirectories
  "include/clay"
  "include/raylib"
  "include/dir_test"
)

set(executables
  "include/clay/multi-compilation-unit.c"
  "include/dir_test/dirtest.c"
)

FetchContent_MakeAvailable(raylib)

add_executable(CANU_I main.c ${executables})

target_compile_options(CANU_I PUBLIC)
target_include_directories(CANU_I PUBLIC ${includedirectories})

target_link_libraries(CANU_I PUBLIC raylib)
if(MSVC)
  set(CMAKE_C_FLAGS_DEBUG "/D CLAY_DEBUG")
else()
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Werror -DCLAY_DEBUG -fsanitize=address")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
endif()

add_custom_command(
        TARGET CANU_I POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources)