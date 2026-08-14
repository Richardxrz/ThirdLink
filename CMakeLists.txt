cmake_minimum_required(VERSION 3.21)

project(thirdlink VERSION 1.0
                  LANGUAGES CXX)

add_subdirectory(thirdlink)

find_package(raylib CONFIG REQUIRED)
target_compile_features(thirdlink PRIVATE cxx_std_20)
target_link_libraries(thirdlink PRIVATE raylib::raylib)
