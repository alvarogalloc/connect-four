add_executable(game-test game-test.cpp)
find_package(ut CONFIG REQUIRED)
target_compile_features(game-test PRIVATE cxx_std_20)
target_link_libraries(game-test PRIVATE gamelib Boost::ut)
add_test(
  NAME game-test
  COMMAND $<TARGET_FILE:game-test>
  )
