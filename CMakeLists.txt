cmake_minimum_required(VERSION 3.0)
project(curse_impact)
include(ExternalProject)

message("${CMAKE_SOURCE_DIR}")


file(GLOB_RECURSE source RELATIVE "${CMAKE_SOURCE_DIR}" "src/*.cpp")
#file(GLOB pugixml RELATIVE "${CMAKE_SOURCE_DIR}" "pugixml/src/*.cpp")


#add_library(ncurses IMPORTED)
ExternalProject_Add(pugixml 
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/pugixml"
  INSTALL_DIR "${CMAKE_BINARY_DIR}/pugixml"
  CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}" 
    "-DCMAKE_INSTALL_INCLUDEDIR=${CMAKE_BINARY_DIR}/pugixml/inc/pugixml"
    "-DCMAKE_INSTALL_LIBDIR=${CMAKE_BINARY_DIR}/pugixml/bin"
)


ExternalProject_Get_Property(pugixml install_dir)
message("EXt install dir: " ${install_dir})
include_directories("${CMAKE_SOURCE_DIR}/inc" "${install_dir}/inc")

message("BINARY: " "${CMAKE_BINARY_DIR}")

set (CMAKE_CXX_FLAGS "-std=c++14 -ggdb -pedantic -Wall -Werror -Og")

message("MESSAGE: ${source}")

add_definitions(-DRES_DIR="${CMAKE_SOURCE_DIR}/res")

add_executable(curse_impact ${source})

target_link_libraries(curse_impact "-lncurses")
target_link_libraries(curse_impact "${install_dir}/bin/libpugixml.a")
