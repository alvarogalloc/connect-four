project(rooster CXX)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_library(centurion)
target_sources(centurion PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES
                              src/centurion.cpp)
target_link_libraries(
  centurion
  PUBLIC
    # $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
    $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
)

add_library(ginseng)
target_sources(ginseng PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES
                              src/ginseng.cpp)



add_library(rooster)
file(GLOB MODULE_FILES src/rooster.cpp src/time_utils.cpp src/logging.cpp)
target_sources(rooster PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES
                              ${MODULE_FILES})
# target_precompile_headers(rooster PRIVATE <entt/entt.hpp>)

target_link_libraries(
  rooster PUBLIC ginseng centurion rooster_project_warnings
                 rooster_project_options spdlog::spdlog fmt::fmt)
