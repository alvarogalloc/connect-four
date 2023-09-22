# give me two commands, one that runs clang-tidy with compilation database in the build directory
# and then runs cppcheck on the source files

# and the other runs clang-format on the source files

# clang-tidy
lint:
	clang-tidy -p build/compile_commands.json game/src/*.cpp
	cppcheck --enable=all --inconclusive --std=c++20 game/src/*.cpp

# clang-format
format:
	clang-format -i game/src/*.cpp

config:
	cmake build

build: config
	cmake --build build

# make format the default target
.DEFAULT_GOAL := build
