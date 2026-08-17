# 知识库 11：Debug 排障方法论（授人以渔）

> 归属阶段：全程。读报错、分层定位、隔离变量——排障的元技能，附 CI 三平台实战案例。

## 一、debug 的元技能（四步）

### 1. 读报错：从下往上读，三层定位

    最后一行 = 错误摘要（Process completed with exit code 1）
    往上一段 = 具体错误（CMake Error at CMakeLists.txt:3、Unable to locate package ...）
    再往上   = 上下文（哪个命令、哪个平台、哪个步骤触发）

口诀：先看最后一行「挂了」，往上看「为什么挂」，再往上「在哪挂」。

### 2. 分层定位：问题出在哪一层

    依赖层：conan/apt/pip —— 包缺失、权限、版本冲突
    配置层：cmake/preset/toolchain —— 找不到包、找不到编译器、路径错
    编译层：编译器 —— 语法错、链接错、头文件缺失
    平台层：系统生态 —— X11 碎片库、MSVC vcvars、macOS framework

先判断哪一层，再进哪一层查——别在编译层死磕一个依赖问题。

### 3. 隔离变量：一次只改一个

不要同时改三个「可能的原因」——改一个 → 验证 → 再改下一个。
否则分不清是哪个修好的，也不知道是哪个又搞坏了。

### 4. 报错信息自包含（答案往往就在报错里）

    conan 报错列全包名 → 抄下来装（我们 6 个坑里 3 个靠这个）
    cmake 报错 "cl not found in PATH" → 就是 PATH 问题
    apt 报错 "Unable to locate package  包名"（双空格）→ 包名带空格 → 续行符问题

## 二、两条排障原则

1. 谁有权限谁干活：CI 里 sudo 你有、conan 没有 → 手动 sudo 装，conan 跳过；
2. 底层条件 > 配置开关：与其赌 mode=disabled 生效，不如让「缺失」这个条件不成立（装全包）。

## 三、CI 三平台实战案例（完整链条）

| 坑 | 报错关键行 | 根因 | 修复 |
|---|---|---|---|
| preset 名不一致 | No such configure preset "conan-debug" | Windows VS 多配置生成 conan-default | 统一强制 Ninja |
| libgl-dev 缺失 | System requirements: libgl-dev are missing | OpenGL 系统库没装 | 加 libgl1-mesa-dev |
| apt 权限失败 | Could not open lock file (Permission denied) | conan 无 root | mode=disabled + 手动装 |
| apt 找不到包 | Unable to locate package  包名（双空格） | 续行符后空格 | YAML > folded |
| cl 找不到 | CMAKE_CXX_COMPILER: cl is not found in PATH | MSVC 要 vcvars 设 PATH | ilammy/msvc-dev-cmd |
| libxcb-util-dev 缺失 | apt-get install libxcb-util-dev failed | 漏一个包 | 补进 apt-get |

## 四、debug checklist（遇到问题先问）

1. 错误信息最后一行说了什么？
2. 哪一层（依赖/配置/编译/平台）？
3. 本地能复现吗？CI 和本地差在哪？
4. 一次只改一个了吗？
5. 报错里是否已经给了答案（包名/路径/变量名）？
