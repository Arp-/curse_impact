cmake_minimum_required(VERSION 3.8)
project(curse_impact)

message("${CMAKE_SOURCE_DIR}")

include_directories("${CMAKE_SOURCE_DIR}/inc" "${CMAKE_SOURCE_DIR}/pugixml/src")

file(GLOB_RECURSE source RELATIVE "${CMAKE_SOURCE_DIR}" "src/*.cpp")
file(GLOB pugixml RELATIVE "${CMAKE_SOURCE_DIR}" "pugixml/src/*.cpp")

#add_library(ncurses IMPORTED)

set (CMAKE_CXX_FLAGS "-std=c++14 -ggdb -pedantic -Wall -Werror -Og")

message("MESSAGE: ${source}")

add_definitions(-DRES_DIR="${CMAKE_SOURCE_DIR}/res")

add_executable(curse_impact ${source} ${pugixml})

target_link_libraries(curse_impact "-lncurses")
