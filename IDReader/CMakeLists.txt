cmake_minimum_required(VERSION 3.27)
project(clay)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(APPLE)
  enable_language(OBJC)
endif()

add_subdirectory("main_app")