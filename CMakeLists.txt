cmake_minimum_required(VERSION 3.10)
project(PlatformGame VERSION 0.1 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add the executable
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(PlatformGame ${SOURCES})

# Find and link SDL2 and its components
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(PlatformGame PRIVATE $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main> $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)

find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(PlatformGame PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)

find_package(SDL2_mixer CONFIG REQUIRED)
target_link_libraries(PlatformGame PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)