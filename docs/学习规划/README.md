# 学习规划（项目内镜像）

本目录是桌面《C++学习规划》文档体系的同步镜像，供开发时随手查阅。

## 来源（桌面为准）

- 主文档：C:\Users\3mz\Desktop\C++学习规划.md
- 知识库：C:\Users\3mz\Desktop\C++学习资料\（01-10 共 10 个主题文档）
- 完整旧版：C:\Users\3mz\Desktop\C++学习与工程化指南-完整版备份.md

## 同步规则

1. **修改以桌面为准**：所有编辑都在桌面文档上进行；
2. **每次修改后同步**：修改桌面文档后，将对应文件复制到本目录（覆盖）；
3. 本目录文件是**只读镜像**，不要在项目里直接改这里的文档（改了也会被下次同步覆盖）；
4. 同步命令（PowerShell）：

       $dst = 'E:\code\c++\ThirdLink\docs\学习规划'
       Copy-Item 'C:\Users\3mz\Desktop\C++学习规划.md' -Destination $dst -Force
       Copy-Item 'C:\Users\3mz\Desktop\C++学习资料\*' -Destination $dst -Recurse -Force

## 首次同步日期

2026-08-13（桌面文档 + 知识库 + 完整版备份）

## 目录内容

    C++学习规划.md                     主文档（学习主线：理念/定位/路线图/阶段详情/书单）
    C++学习与工程化指南-完整版备份.md   未拆分的旧完整版（历史参考）
    01-语言核心概念.md                 阶段 0：复合字面量/constexpr/inline/栈与堆/range-for
    02-容器与STL.md                    阶段 0/3：容器详解 + 使用频率排行
    03-工具链与构建系统.md             阶段 5：编译器/构建系统/跨平台工具
    04-Conan包管理.md                  阶段 5：Conan 上手 + gcc/clang 取舍
    05-数据结构学习.md                 阶段 4：环节 1+3 资源清单
    06-设计观念与参考.md               全程：YAGNI/需求到方案对照
    07-ThirdLink改造清单.md            全程：代码/构建/工程化任务清单
    08-开源工程化.md                   阶段 7：开源四阶段流程与礼仪
    09-Linux开发环境.md                阶段 8：Windows 到 Linux 迁移
    10-ThirdLink项目规划.md            全程：定位/路线图/目录结构/API 设计原则
