cmake_minimum_required(VERSION 3.21)

project(thirdlink VERSION 1.0
                  LANGUAGES CXX)

set(RAYLIB_ROOT "E:/Lib/raylib-6.0_win64_mingw-w64/raylib-6.0_win64_mingw-w64/")
set(RAYLIB_INC ${RAYLIB_ROOT}/include)
set(RAYLIB_LIB ${RAYLIB_ROOT}/lib/libraylib.a)

add_subdirectory(thirdlink)

target_include_directories(thirdlink PRIVATE ${RAYLIB_INC})
target_link_libraries(thirdlink
  PRIVATE
    ${RAYLIB_LIB}
    winmm
)
target_compile_features(thirdlink PRIVATE cxx_std_20)
