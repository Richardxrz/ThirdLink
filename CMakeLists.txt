cmake_minimum_required(VERSION 3.21)

project(thirdlink VERSION 1.0
                  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib CONFIG REQUIRED)

add_subdirectory(thirdlink)

target_link_libraries(thirdlink
  PRIVATE
    raylib
)
target_compile_features(thirdlink PRIVATE cxx_std_20)

if (MSVC)
  target_compile_options(thirdlink PRIVATE /W4)
else()
  target_compile_options(thirdlink PRIVATE -Wall -Wextra -Wpedantic)
endif()
