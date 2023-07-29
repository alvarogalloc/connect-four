include(FetchContent)
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

# enable c++20 modules api
# turn on the experimental API
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API
    "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  message(FATAL_ERROR "There is not yet support for modules in gcc ")
endif()


# Add project_options from https://github.com/aminya/project_options Change the
# version in the following URL to update the package (watch the releases of the
# repository for future updates)
set(PROJECT_OPTIONS_VERSION "v0.29.0")
FetchContent_Declare(
  _project_options
  URL https://github.com/aminya/project_options/archive/refs/tags/${PROJECT_OPTIONS_VERSION}.zip
)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)

# install vcpkg stuff
run_vcpkg()

option(USE_SANITIZERS "Enable address sanitizer" OFF)
option(USE_CPPCHECK "ENABLE_CPPCHECK" OFF)
option(USE_LLD "Enable a better linker" ON)

if(USE_SANITIZERS)
  set(ENABLE_SANITIZER_ADDRESS "ENABLE_SANITIZER_ADDRESS")
  set(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "ENABLE_SANITIZER_UNDEFINED_BEHAVIOR")
endif()
if(USE_CPPCHECK)
  set(ENABLE_CPPCHECK "ENABLE_CPPCHECK")
endif()
if(USE_LLD)
  set(ENABLE_LLD "LINKER \"lld\"")
endif()

project_options(
  PREFIX
  "rooster"
  ${ENABLE_CPPCHECK}
  ${ENABLE_SANITIZER_ADDRESS}
  ${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
  ${ENABLE_LLD})

