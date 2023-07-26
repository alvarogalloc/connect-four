cmake_minimum_required(VERSION 3.24)
project(connect-four)
set(CMAKE_CXX_STANDARD 20)

include(cmake/setup.cmake)
include(cmake/CPM.cmake)

add_subdirectory(rooster)
add_subdirectory(game)
