project(game)

# find modules (all files in src but main.cpp)
set(MODULE_FILES  src/random.cpp src/time_utils.cpp src/board.cpp src/setup.cpp src/logging.cpp)

add_executable(game src/main.cpp)
target_sources(game PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES
                              ${MODULE_FILES})
target_link_libraries(game PRIVATE rooster)
target_compile_features(game PRIVATE cxx_std_20)
# add -ftime-trace in clang
target_compile_options(game PRIVATE $<$<CXX_COMPILER_ID:Clang>:-ftime-trace>)
# copy assets after build
# target is under build/[target]/game
add_custom_command(
  TARGET game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets
          ${CMAKE_CURRENT_BINARY_DIR}/assets)
