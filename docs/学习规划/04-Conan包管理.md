# 知识库 04：Conan 包管理

> 归属阶段：阶段 5。Conan 概念、profile、快速上手、gcc vs clang 成熟度、官方 LLVM/Clang 工具链。

## 6.1 Conan 是什么

跨平台 C/C++ 包管理器：声明依赖（conanfile）→ 下载或从源码编译依赖 → 生成 CMake 配置 → 你的项目 find_package 使用。
你机器环境：Conan 2.29.1、CMake 4.3.1、ninja 1.13.2 已装好；ConanCenter 上有 raylib/6.0（依赖 glfw/3.4 + opengl/system）。

## 6.2 profile 是什么

描述「用什么编译器、什么系统、什么架构、什么标准」的配置。不同 profile = 不同构建配置。

## 6.3 一步到位的 profile（推荐 gcc，附 clang 备选）

关键结论（已查证官方文档 + GitHub issue #18634）：在 Windows 上让 Conan 直接用 clang 编译依赖包会频繁失败，
gcc + MinGW 是官方明确支持、最稳的路径。所以：
- 首选：用 gcc profile 让 Conan 编译依赖；
- 若你坚持自己的项目用 clang：依赖用 gcc 编译（libstdc++ ABI），自己的代码用 clang 编译再链接，两者 ABI 兼容。

推荐 gcc profile（保存到 C:\Users\3mz\.conan2\profiles\mingw-gcc）：
    [settings]
    os=Windows
    arch=x86_64
    compiler=gcc
    compiler.version=16
    compiler.libcxx=libstdc++11
    compiler.cppstd=20
    compiler.threads=posix
    compiler.exception=seh
    build_type=Release

    [buildenv]
    PATH+=(path)E:/Msys/mingw64/bin

备选 clang profile（保存为 mingw-clang，注意：用它编译第三方依赖有失败风险）：
    [settings]
    os=Windows
    arch=x86_64
    compiler=clang
    compiler.version=22
    compiler.libcxx=libstdc++11
    compiler.cppstd=20
    build_type=Release

    [buildenv]
    PATH+=(path)E:/Msys/mingw64/bin

    [conf]
    tools.build:compiler_executables={"c": "E:/Msys/mingw64/bin/clang.exe", "cpp": "E:/Msys/mingw64/bin/clang++.exe"}

说明：compiler.version 取 g++/clang++ --version 的主版本号；threads/exception 是 MinGW 特有设置（MSYS2 默认 posix+seh）。

## 6.4 针对 ThirdLink 的快速步骤

第1步：写 conanfile.txt（项目根目录）
    [requires]
    raylib/6.0

    [generators]
    CMakeDeps
    CMakeToolchain

    [layout]
    cmake_layout

第2步：安装依赖（Debug/Release 各一次）
    cd E:\code\c++\ThirdLink - 副本
    conan install . --build=missing -pr:h=mingw-gcc -s build_type=Debug   -of build
    conan install . --build=missing -pr:h=mingw-gcc -s build_type=Release -of build

    --build=missing 会从源码编译 raylib + glfw，第一次需几分钟，之后有缓存。
    这步会在 build/ 生成 CMakeUserPresets.json（含 conan-debug / conan-release 预设）和依赖的 CMake 配置文件。
    关键坑：conan 通过 PATH 找 g++，先执行 $env:PATH = "E:\Msys\mingw64\bin;" + $env:PATH（或加到系统 PATH）。

第3步：重写 CMakeLists.txt（合并并删掉所有硬编码）
    cmake_minimum_required(VERSION 3.20)
    project(thirdlink LANGUAGES CXX)

    find_package(raylib CONFIG REQUIRED)

    add_executable(thirdlink thirdlink/thirdlink.cpp)
    target_compile_features(thirdlink PRIVATE cxx_std_20)
    target_link_libraries(thirdlink PRIVATE raylib::raylib)

    （已查证 ConanCenter raylib recipe：CMake target 名就是 raylib::raylib，winmm/gdi32/opengl32 经 glfw 自动传递。）
    （删掉 RAYLIB_ROOT 硬编码、thirdlink/CMakeLists.txt 里的 RAYLIB_DIR。）

第4步：用 Conan 生成的预设构建
    cmake --preset conan-debug
    cmake --build --preset conan-debug

第5步：清理
    - 旧的 toolchains/*.cmake 和手写 presets 可删（Conan 的 CMakeToolchain 自己处理编译器和 flags）；
    - 保留则修掉 Libr 拼写错误、gcc-release 的 Debug 复制粘贴 bug；
    - git init + .gitignore（忽略 build/、.cache/）。

## 6.5 Conan 资源

- 官方文档：docs.conan.io/2（权威，2.x 版本文档）；
- Conan Center Index：github.com/conan-io/conan-center-index（看某个包的 recipe 怎么写、提供什么 target）；
- 官方示例仓库：github.com/conan-io/examples2（大量可运行示例）；
- 教程：docs.conan.io/2/tutorial.html（官方 Tutorial，跟着走一遍即可入门）。

---


---

结论：问题不在 clang 本身，而在「Conan 依赖生态对 clang+MinGW 这个组合的覆盖差」。具体原因：
    1. clang 在 Windows 上有两副面孔：clang-cl（MSVC ABI）vs clang（GNU/MinGW ABI）。Conan 在 Windows 上
       默认把 compiler=clang 当成 clang-cl（MSVC），要正确识别 MinGW 版 clang 需额外配置，检测很脆弱。
    2. 预编译包几乎都是给 MSVC 或 MinGW-gcc 的，clang+MinGW 组合几乎没有现成二进制，只能 --build=missing
       从源码编译，而很多 recipe 的构建脚本只按 MSVC / gcc-MinGW 写分支，遇到 clang 就失败
       （官方 issue #18634 原话：用 clang 时「大多数情况下安装 conan 包都会失败」）。
    3. recipe 里常见 if(MSVC)/if(MINGW) 判断，clang+MinGW 会同时命中 __clang__ 和 __MINGW32__，但 CMake
       编译器识别是「Clang」，导致分支错配。

注意范围：这只针对 Windows+MinGW。在 Linux 上 clang 是一等公民；在 macOS 上 Apple Clang 是 Conan 的
默认编译器。所以你以后在 Linux/macOS 用 clang + Conan 完全没问题。

正确姿势（Windows）：依赖用 gcc profile 编译（libstdc++ ABI），你自己的代码用 clang 编译再链接，
两者 ABI 兼容（都用 libstdc++）。



---

## 9.1 核心澄清：Windows 上「独立 clang」不是第三种 ABI

Windows 上的 clang 只有两种运行时（ABI），没有「独立于一切」的第三种：
    1. MSVC 运行时（msvcrt + MSVC STL）—— LLVM 官方下载的 clang 默认就是这个（需装 VS Build Tools + Windows SDK）。
    2. MSYS2 运行时（libstdc++6.dll）—— MSYS2 里的 clang（你现在用的）。

关键点：clang 就是同一个 LLVM clang，区别只在 target/运行时。官方下载的「独立」clang 只是
「不依赖 MSYS2」，但仍依赖 MSVC 运行时（要装 VS Build Tools）。所以「独立」是相对 MSYS2 而言，不是绝对独立。

Conan 里区分两者的关键设置是 compiler.runtime：官方 clang 有 compiler.runtime（dynamic/static），
MSYS2 clang 没有（它用 libstdc++）。

## 9.2 Conan 对官方 clang（MSVC 运行时）的支持：好，但需 VS Build Tools

- 因为复用 MSVC 生态（Windows SDK + msvcrt），ConanCenter 有给 compiler=clang（MSVC runtime）的预编译包，
  覆盖比 MinGW-clang 好得多；
- 但仍略少于纯 MSVC 和 MinGW-gcc；
- 代价：必须先装 Visual Studio 2022 Build Tools（含 MSVC 工具集 + Windows SDK）。

## 9.3 「独立版能避免大部分适配问题吗」——直接回答

- gcc：没有「更独立」的 gcc；MinGW-w64 gcc 就是 Windows 标准 gcc，Conan 支持最好，无适配问题。
- clang：换官方 LLVM clang（MSVC runtime）能避开 MinGW-clang 的坑，但换成依赖 VS Build Tools，
  且二进制覆盖仍略少于纯 MSVC。所以「独立版」有帮助，但不能「避免大部分」，只是换了个生态。
- 适配问题的根源是 ABI/运行时，不是「独立 vs 打包」。

Windows 上 Conan 编译依赖的成熟度排序（从好到差）：
    1. MSVC (cl.exe)
    2. MinGW-w64 gcc
    3. LLVM clang（MSVC 运行时）
    4. MinGW clang（libstdc++）—— 最差

跨平台结论：Linux / macOS 上 clang 是一等公民（macOS 默认 Apple Clang），无脑用；
Windows 上想最少踩坑就选 MSVC 或 MinGW-gcc，clang 是第三选择。

## 9.4 围绕「官方 LLVM/Clang」的完整工具链（推荐配置）

前提：装 Visual Studio 2022 Build Tools（工作负载选「使用 C++ 的桌面开发」→ 得到 MSVC 工具集 + Windows SDK），
然后从 llvm.org（或 winget install LLVM.LLVM）下载官方 clang。

profile（保存为 llvm-clang）：
    [settings]
    os=Windows
    arch=x86_64
    build_type=Release
    compiler=clang
    compiler.version=22
    compiler.cppstd=20
    compiler.runtime=dynamic
    compiler.runtime_type=Release
    compiler.runtime_version=v143

    [buildenv]
    PATH=+(path)C:/LLVM/bin

    [conf]
    tools.cmake.cmaketoolchain:generator=Ninja

    [tool_requires]
    ninja/[*]

说明：
    - compiler.version 用 clang --version 的主版本号（你机器 MSYS2 clang 是 22，官方 clang 也是 22 系）；
    - compiler.runtime_version 填你装的 MSVC 工具集版本，VS2022 = v143；
    - 官方 clang 的 GNU-like 前端是 clang++，CMake 会自动加 --dependent-lib=msvcrt 链接动态运行时。

构建：
    conan install . --build=missing -pr=llvm-clang -of build
    cmake --preset conan-release
    cmake --build --preset conan-release

（clang-cl 前端变体：在 [conf] 里加 tools.build:compiler_executables={"c":"clang-cl","cpp":"clang-cl"}，其余不变。）

## 9.5 给你的一句话建议

Windows 上你现在的 MinGW 环境里，gcc 是 Conan 最省心的选择；想用 clang 就：
- 要稳 → 官方 LLVM clang + VS Build Tools（本节 profile）；
- 要轻 → 依赖用 gcc 编译（libstdc++ ABI），自己的代码用 clang 编译再链接，两者 ABI 兼容。
Linux/macOS 上直接用 clang，没有任何这些麻烦。

---


## 实战记录：ThirdLink 接入 Conan 踩坑（已验证跑通）

最终流程：
    conan install . --build=missing -pr:h=mingw-gcc -s build_type=Debug
    cmake --preset conan-debug
    cmake --build --preset conan-debug

踩过的坑（每个都可能导致 find_package 失败或构建失败）：
1. CMake target 名是 raylib（不带 ::）—— conan install 的提示会直接告诉你
   find_package(raylib) / target_link_libraries(... raylib)；
2. profile 必须加 [conf] tools.cmake.cmaketoolchain:generator=Ninja，
   否则 conan 默认 MinGW Makefiles（需要 sh.exe，找不到就出问题）；
3. conan install 不要乱加 -of/--output-folder，否则和 cmake_layout 叠加出
   build/build/Debug 嵌套路径；
4. CMakeUserPresets.json 里残留的旧 include 路径要清理，
   否则两个 CMakePresets.json 里的 conan-debug 预设重名冲突；
5. compile_commands.json：在 CMakeLists.txt 里 set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 最省事；
6. .clangd 的 CompilationDatabase 要指向 conan 的构建目录（build/Debug）。


## Preset 文件体系：CMakePresets.json vs CMakeUserPresets.json

| | CMakePresets.json | CMakeUserPresets.json |
|---|---|---|
| 谁写 | 你手写（项目级） | Conan 自动生成（用户级） |
| 提交 git？ | 提交（团队共享） | 不提交（含绝对路径，机器特定） |
| 内容 | 项目级构建配置 | conan-debug / conan-release 预设 |
| 谁读 | 协作者 + CI | 只有你 |

- conan-debug/conan-release 不用手写，conan install 每次自动重新生成；
- CMakePresets.json 仍要维护（可以很薄），它是项目级公开声明；
- CMakeUserPresets.json 应加入 .gitignore（含绝对路径 binaryDir）；
- 链条：conan install（生成 user preset）→ cmake --preset conan-debug（读取）→ 插件 :CMakeBuild 执行；
- preset 是配置快照，插件是命令封装，两者是「生成配置」和「执行命令」的关系。



## Conan 工具链管理机制（profile → toolchain → CMake）

配置信息的唯一来源是 profile（~/.conan2/profiles/xxx），生成文件是派生产物，永远不改。

conan_toolchain.cmake 是「翻译器」：把 profile 翻译成 CMake 的 set 语句，分 block：
    arch_flags block  -m64                 ← profile 的 arch=x86_64
    cppstd block      CMAKE_CXX_STANDARD 20 ← profile 的 compiler.cppstd=20
    compilers block   空（关键！）         ← 编译器在 PATH 里就不写路径
    find_paths block  CMAKE_PREFIX_PATH    ← conan 缓存的依赖包路径（find_package 靠它）

为什么 compilers block 是空的：profile 的 [buildenv] PATH 已把 g++ 放进 PATH，
CMake 能按名字找到，conan 就无需显式写 CMAKE_CXX_COMPILER。

这就是「非硬编码」原理：路径只写在 profile 的 PATH 一处，而不是散落在 preset 里。

Conan 两大职责：
    依赖管理   [requires] raylib/glfw  → CMakeDeps 生成 raylib-config.cmake
    工具链管理 profile（compiler/flags/PATH）→ CMakeToolchain 生成 conan_toolchain.cmake

消除硬编码：删掉手写 CMakePresets 里的旧预设（CMAKE_CXX_COMPILER 硬编码），
工具链全交给 profile——换编译器 = 改 profile + 重新 conan install。



## generator 概念 + CMakeDeps / CMakeToolchain 分工

generator = conan 的「生成文件」步骤：下载依赖后，生成文件给构建系统用（conan 不绑定构建系统，靠 generator 适配）。

两个主力 generator 各管一半：
    CMakeDeps      → 生成 raylib-config.cmake / raylibTargets.cmake / glfw-config.cmake
                     解决 find_package(raylib) 去哪找：头文件、库文件、传递依赖（glfw/opengl/winmm）
    CMakeToolchain → 生成 conan_toolchain.cmake + CMakePresets.json
                     解决工具链：编译器、标准、flags、CMAKE_PREFIX_PATH

分工对应 conan 两大职责：CMakeToolchain 管工具链（profile），CMakeDeps 管依赖（requires）。

关键坑：CMakeDeps 生成的 target 名就是你要链接的名字——默认是 raylib（不带 ::），
不是 raylib::raylib（那是 recipe 的别名，你的配置没启用就找不到）。



## conanfile.txt 的 [layout]：cmake_layout 是什么

layout 定义的是「conan 生成物放哪 + 构建发生在哪个目录」，不是安装位置。

cmake_layout = Conan 2 内置的、为 CMake 项目设计的标准布局：
    build/Debug/      ← 构建目录（build_type=Debug）
    build/Debug/generators/  ← conan 生成物（toolchain/config/presets）
    build/Release/    ← 构建目录（build_type=Release）

作用：统一 conan 和 cmake 的路径约定——conan 往 build/{build_type}/generators 写，
cmake 的 preset 指向 build/{build_type} 构建。

坑：-of build 会和 cmake_layout 叠加成 build/build/Debug（之前踩过）；
「安装（install）」是另一个概念（cmake install 装到系统），与 layout 无关。

