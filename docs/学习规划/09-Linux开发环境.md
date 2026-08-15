# 知识库 09：Linux 开发环境（Windows → Linux 迁移）

> 归属阶段：全程并行。Windows 上开发，Linux 上发布和长期使用。

## 一、为什么迁 Linux

- 开源工具生态原生（编译器/包管理/CI 在 Linux 上最顺）；
- 服务器/嵌入式部署目标平台就是 Linux；
- 终端工作流（bash + nvim + git）效率上限更高。

## 二、迁移路径（渐进，别一步到位）

1. 第 1 步：WSL2（Windows 里跑 Linux，开发体验最接近）
   - 安装：wsl --install -d Ubuntu
2. 第 2 步：代码在 WSL2 里构建，编辑器在 WSL 里跑 nvim；
3. 第 3 步：熟悉后双系统 / 云服务器（服务器上跑 CI/部署）。

## 三、Windows ↔ Linux 工具对照表

    Windows                Linux
    PowerShell             bash / zsh
    E:\path                /home/user/path
    cmd 内建命令            ls/cd/cp/mv/rm 等
    g++.exe (MSYS2)        g++ / clang++
    CMake + Ninja          相同（跨平台）
    Conan                  相同（跨平台）
    Visual Studio          VS Code / CLion
    VS debugger            gdb / lldb
    （无）                  valgrind（内存检测，Linux 最强项）
    winget / MSYS2         apt / pacman
    （无）                  systemd（服务管理）

## 四、Linux 必学命令（按类，用到再深入）

- 文件系统：ls / cd / pwd / cp / mv / rm / mkdir / find / grep
- 权限：chmod / chown / sudo
- 进程：ps / top / kill / & 后台运行 / nohup
- 网络：ping / curl / ssh / scp
- 包管理：apt install / apt update / apt search
- 文本：vim / nvim / cat / head / tail / less
- Git 在 Linux 与 Windows 用法完全一致

## 五、Linux 上装开发工具链（Ubuntu 示例）

    sudo apt update
    sudo apt install build-essential cmake ninja-build gdb
    sudo apt install clang clang-format clang-tidy
    pip install conan          （或 apt install conan）
    （nvim 用官方 AppImage 或 apt 安装）

## 六、学习资源

- Linux Journey（linuxjourney.com，免费中文/英文，按模块学）；
- 《The Linux Command Line》（TLCL，免费在线英文，经典）；
- 《鸟哥的 Linux 私房菜》基础篇（中文经典，厚，按需翻）。

## 七、分阶段计划（与项目并行）

    第 1 周：装 WSL2 + Ubuntu，学 ls/cd/文件系统/权限，装工具链；
    第 2 周：在 WSL2 里把 ThirdLink 构建起来（cmake + conan + ninja）；
    第 3 周：学 git 命令行 + bash 脚本基础（写一个 build.sh）；
    第 4 周起：日常开发切到 WSL2，Windows 只留验证用；
    长期：CI 加 ubuntu 平台，习惯在 Linux 上调试（gdb/valgrind）。
