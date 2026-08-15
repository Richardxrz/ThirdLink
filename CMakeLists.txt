cmake_minimum_required(VERSION 3.21)

project(thirdlink VERSION 1.0
                  LANGUAGES CXX)

# 生成 compile_commands.json（clangd 用）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 依赖由 Conan 管理：先 conan install，再 find_package
find_package(raylib CONFIG REQUIRED)

add_subdirectory(thirdlink)

target_link_libraries(thirdlink
  PRIVATE
    raylib   # Conan CMakeDeps 生成的 target 名（不带 ::）
)
target_compile_features(thirdlink PRIVATE cxx_std_20)

if (MSVC)
  target_compile_options(thirdlink PRIVATE /W4)
else()
  target_compile_options(thirdlink PRIVATE -Wall -Wextra -Wpedantic)
endif()
