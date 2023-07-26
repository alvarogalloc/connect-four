project(rooster CXX)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

if (NOT TARGET centurion)
CPMAddPackage(
  NAME centurion
  URL https://github.com/albin-johansson/centurion/archive/refs/tags/v7.3.0.zip
  VERSION 7.3.0
  DOWNLOAD_ONLY YES)

if(centurion_ADDED)
  add_library(centurion INTERFACE)
  target_include_directories(centurion SYSTEM INTERFACE "${centurion_SOURCE_DIR}/src")

  target_link_libraries(
    centurion
    INTERFACE
      # $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
      $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
      $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
      $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
      $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
  )
else()
  message(FATAL ERROR "Could not download centurion")
endif()
endif()



add_library(rooster)
file(GLOB MODULE_FILES src/*.cpp)
target_sources(rooster PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES
                              ${MODULE_FILES})
# target_precompile_headers(rooster PRIVATE <entt/entt.hpp>)

target_link_libraries(
  rooster PUBLIC centurion rooster_project_warnings
                 rooster_project_options spdlog::spdlog fmt::fmt)
