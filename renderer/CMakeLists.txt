cmake_minimum_required(VERSION 3.0)
## Note calling this a renderer is like calling notepad a text editor
## It feels like i'm mocking graphics programmers, but oh well
project(renderer)

include(ExternalProject)
find_package(Git REQUIRED)

message ("${CMAKE_SOURCE_DIR}")


message("MESSAGE: ${source}")

#add_library(renderer "${source}")

#add_executable(renderer_test ${source})

ExternalProject_Add(
  catch
  PREFIX ${CMAKE_BINARY_DIR}/catch
  GIT_REPOSITORY "https://github.com/philsquared/Catch"
  GIT_TAG "v1.11.0"
  TIMEOUT 10000
  UPDATE_COMMAND ""
  DOWNLOAD_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON
)

ExternalProject_Get_Property(catch source_dir)
set(CATCH_INCLUDE_DIR ${source_dir}/single_include CACHE INTERNAL "${CMAKE_BINARY_DIR}/catch/")

message("CATCH_INCLUDE_DIR: ${CATCH_INCLUDE_DIR}")
include_directories(${CATCH_INCLUDE_DIR} "${CMAKE_SOURCE_DIR}/inc")
enable_testing(true)


file (GLOB_RECURSE source RELATIVE "${CMAKE_SOURCE_DIR}" "src/*.cpp")

file (GLOB_RECURSE header RELATIVE "${CMAKE_SOURCE_DIR}" "inc/*.hpp")

set (CMAKE_CXX_FLAGS "-std=c++14 -ggdb -pedantic -Wall -Werror -O0")


file (GLOB test_source RELATIVE "${CMAKE_SOURCE_DIR}" "test/*.cpp")
add_executable(run_test ${test_source} ${source})


if (NOT LIBDIR)
  set(LIBDIR "lib")
endif(NOT LIBDIR)

if (NOT INCDIR)
  set(INCDIR "inc")
endif(NOT INCDIR)


#install(TARGETS renderer DESTINATION ${LIBDIR})
install(FILES ${header} DESTINATION ${INCDIR})

