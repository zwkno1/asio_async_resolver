cmake_minimum_required(VERSION 3.10)

project(cares_resolver)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

include_directories(./include)

add_subdirectory(examples)
