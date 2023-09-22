cmake_minimum_required(VERSION 3.24)
project(connect-four)
include(CTest)
enable_testing()
set(CMAKE_CXX_STANDARD 20)

include(cmake/setup.cmake)
include(cmake/CPM.cmake)

add_subdirectory(rooster)
add_subdirectory(game)
add_subdirectory(game-test)
