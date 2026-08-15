# ThirdLink Simulation

三连杆机械臂运动学仿真（学习项目：以工程化流程为主要目标，不追求功能完善）

## 构建

依赖：CMake 3.21+、Ninja、Conan 2.x、gcc / clang（MinGW 或 Linux/macOS 原生）

    conan install . --build=missing -pr:h=mingw-gcc -s build_type=Debug
    cmake --preset conan-debug
    cmake --build --preset conan-debug

产物：build/Debug/thirdlink/thirdlink(.exe)

## 依赖管理

- 依赖（raylib）由 Conan 管理，声明在 conanfile.txt；
- 构建预设由 Conan 的 CMakeToolchain 生成（conan-debug / conan-release），见 CMakeUserPresets.json；
- 不需要手动配置 raylib 路径（无硬编码）。

## 项目简介

这是一个用于连杆运动学仿真的库。当前进度：静态三连杆绘制（见 docs/学习规划 的路线图）。
