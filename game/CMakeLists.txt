project(game)

# find modules (all files in src but main.cpp)
set(MODULE_FILES  src/random.cpp src/time_utils.cpp src/board.cpp)

add_executable(game src/main.cpp)
target_sources(game PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES
                              ${MODULE_FILES})
target_link_libraries(game PRIVATE rooster)
target_compile_features(game PRIVATE cxx_std_20)
