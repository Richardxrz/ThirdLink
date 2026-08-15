# C++ 学习规划

> 从 C 到纯 C++，再回嵌入式。主线是「项目驱动」：每个阶段的知识都落在 ThirdLink 项目上，验收标准先行。
> 方法论：**工具优先于理论**——先会用工具（git/cmake/conan/linux），在实践中学理论，理论反过来指导造轮子。
> 知识点细节在「C++学习资料」目录，按需点击链接跳转；完整旧文档见《C++学习与工程化指南-完整版备份.md》。

---

## 一、学习理念（先立规矩）

1. **需求驱动（YAGNI）**：新特性只在项目里出现真实需求时才引入，不做多余设计。
2. **项目驱动**：阶段知识 → 立刻应用到 ThirdLink → 形成肌肉记忆。
3. **验收标准先行**：每阶段结束有一个可验证的产出（0 警告 / 分层完成 / 测试通过）。
4. **容器思维**：先学会「选对结构」，实现细节交给 STL（见阶段 4）。
5. **工具优先于理论**：先掌握工具（编译/构建/依赖/版本控制/终端），理论在实践中间歇插入——
   只有会用工具，才能在理论指导下造轮子。

详细设计观念见 [知识库 06：设计观念与参考](C++学习资料/06-设计观念与参考.md)。

---

## 二、项目定位（ThirdLink）

**一句话**：一个以学习工程化为首要目标的 2D 机械臂运动学/动力学仿真练习项目。

- **首要目标**：完整走一遍工程化流程（版本控制/构建/依赖/测试/CI/文档/发布/开源礼仪），功能「够用即可」；
- 与 matlab / rviz / moveit 不竞争，参考它们的 API 设计思路；
- 未来可能成为 2D 物理/游戏引擎前身 → 核心逻辑与渲染解耦，渲染后端可替换；
- 完整定位/路线图/目录结构见 [知识库 10：ThirdLink 项目规划](C++学习资料/10-ThirdLink项目规划.md)。

---

## 三、总体路线图

阶段 0 语言基础 → 1 类与封装 → 2 所有权 → 3 泛型算法 → 4 数据结构（穿插）→ 5 工程化（持续）→ 7 开源发布；阶段 8 Linux 全程并行；阶段 6 回嵌入式（未来）。

| 阶段 | 主题 | 时间 | 项目作业 | 验收标准 | 知识库 |
|---|---|---|---|---|---|
| 0 | 语言基础 | 2-3 周 | ThirdLink 清理 | 严格编译 0 警告 | 01、02 |
| 1 | 类与封装 | 3-4 周 | Arm 类化、模型/渲染分层 | 模型层无 raylib 依赖 | 01 |
| 2 | 所有权与资源 | 2-3 周 | 重力环境（粒子实体） | 无裸 new/delete | 01 |
| 3 | 泛型与算法 | 3-4 周 | 运动学/矩阵库 + 单测 | FK 计算有测试覆盖 | 02 |
| 4 | 数据结构 | 穿插 2 周 | A* 寻路 | 能跑通简单场景 | 05 |
| 5 | 工程化 | 持续 | Conan 接入、CI | 别人能一键构建 | 03、04、07 |
| 7 | 开源发布 | 1-2 周 | License/README/Release 上线 | 项目正式发布到 GitHub | 08 |
| 8 | Linux 迁移 | 并行 | WSL2 上构建 ThirdLink | Linux 上一键构建 | 09 |
| 6 | 回嵌入式 | 未来 | C 实现链表/环形缓冲/内存池 | 嵌入式可编译 | 03 |

---

## 四、阶段详情

### 阶段 0：语言基础（2-3 周）

**目标**：完成 C 到 C++ 的核心差异认知，把 ThirdLink 从「C 风格」改成「C++ 风格」。

**核心概念**：namespace / 引用 / const 与 constexpr / 重载 / RAII 入门 / array·vector·span / range-for / 匿名 namespace。

**资料**：《A Tour of C++》第 1-6 章 + learncpp 对应章节。

**项目作业**（[改造清单见知识库 07](C++学习资料/07-ThirdLink改造清单.md)）：
- 删死代码（InitJoint/InitLink/DrawJoint/DrawLink/EndEffector 等）；
- 宏 → constexpr；复合字面量 → C++ 聚合初始化；
- 全局 C 数组 → std::vector 成员；文件内函数 → static/匿名 namespace；
- Link::end() 用 [[nodiscard]]。

**验收**：严格编译（-Wall -Wextra -Wpedantic）0 警告。

**知识库**：[01 语言核心概念](C++学习资料/01-语言核心概念.md) · [02 容器与 STL](C++学习资料/02-容器与STL.md)

---

### 阶段 1：类与封装（3-4 周）

**目标**：用类表达机械臂对象，实现「模型与渲染分离」。

**核心概念**：构造/析构、成员函数、const 正确性、拷贝、运算符重载、封装。

**资料**：《A Tour of C++》第 7-10 章。

**项目作业**：
- Segment/Arm 类化（Arm 拥有数据，只通过 arm 访问）；
- 模型层（纯逻辑，无 raylib）与渲染层分离；
- InitWindow/CloseWindow 用 RAII 包装（构造即打开、析构即关闭）。

**验收**：模型层代码不含任何 raylib 调用；Arm 创建即完成初始化（无手工 Init 函数）。

---

### 阶段 2：所有权与资源管理（2-3 周）

**目标**：理解 RAII 与智能指针，让资源管理变成「默认正确」。

**核心概念**：unique_ptr/shared_ptr、移动语义、Rule of Zero、栈与堆（[见知识库 01](C++学习资料/01-语言核心概念.md)）。

**资料**：《A Tour of C++》第 12-13 章 + 《Effective Modern C++》相关条款。

**项目作业**：实现「重力环境」TODO——质量块实体，容器 + 智能指针管理。

**验收**：全项目无裸 new/delete；析构不泄漏。

---

### 阶段 3：泛型与算法（3-4 周）

**目标**：用模板和标准算法表达运动学与矩阵运算。

**核心概念**：模板、lambda、标准算法（sort/find/count/transform）、容器与算法配合（[见知识库 02](C++学习资料/02-容器与STL.md)）。

**资料**：《A Tour of C++》第 14-17 章。

**项目作业**：
- 「运动学库」+「矩阵运算库」TODO（API 参考 GLM）；
- 配 GoogleTest/Catch2 单元测试。

**验收**：正向运动学计算有测试覆盖；矩阵库可独立于渲染层使用。

---

### 阶段 4：数据结构与算法（穿插 2 周）

**目标**：掌握「复杂度分析」和「结构选型」两项核心能力，理解 STL 容器背后的代价。

**核心概念**：Big-O、哈希表、树、堆、图、BFS/DFS/Dijkstra·A*、排序与二分。

**资料**：《Hello 算法》（hello-algo.com）+ Big-O Cheat Sheet + Core Guidelines SL 章节（[完整清单见知识库 05](C++学习资料/05-数据结构学习.md)）。

**项目作业**：给 ThirdLink 写一个 A* 寻路（网格场景）。

**验收**：能跑通简单场景；能口算自己代码的复杂度、说出每个集合为什么选这个容器。

---

### 阶段 5：工程化（持续）

**目标**：让项目可以被任何人、任何平台一键构建。

**核心概念**：CMake + Ninja + Presets、Conan 包管理、clang-format/tidy、CI 三平台矩阵。

**资料**：[知识库 03：工具链与构建系统](C++学习资料/03-工具链与构建系统.md) · [知识库 04：Conan 包管理](C++学习资料/04-Conan包管理.md)。

**项目作业**：
- [x] 接入 Conan（替换硬编码 raylib 路径）—— 2026-08-13 已完成，构建跑通；
- [x] CMakePresets 修复（Libr 拼写）—— 已完成；
- [x] README（含一键构建说明）—— 已完成；git 仓库已存在；
- [x] GitHub Actions 三平台矩阵（windows/linux/macos）—— CI 文件已写好（.github/workflows/ci.yml），推送 GitHub 后生效；
- [x] 编译 0 警告门禁（-Wall -Wextra -Wpedantic）—— 你已自己加入 CMake，验证 0 警告通过。
- [ ] 把项目推送到 GitHub 并确认 CI 三平台全绿 —— 待做（需要建仓库 + push）。

**验收**：按 README 能在三个平台一键构建；编译 0 警告。

---

### 阶段 7：开源发布（1-2 周，ThirdLink 的首个开源项目）

**目标**：把 ThirdLink 正式开源，走完初始化 → 开发 → 发布 → 维护的完整流程。

**核心概念**：License（MIT）、README、Conventional Commits、SemVer、CHANGELOG、GitHub Actions、Issue/PR 模板、Doxygen API 文档。

**资料**：[知识库 08：开源工程化](C++学习资料/08-开源工程化.md)。

**项目作业**：
- git init + LICENSE(MIT) + README + .gitignore + .editorconfig；
- 提交规范（Conventional Commits，feat:/fix:/docs:...）；
- GitHub Actions CI（三平台编译 + 测试）；
- Doxygen API 文档（公共头文件注释齐全）；
- 首个 v0.1.0 release（tag + Release notes + CHANGELOG）。

**验收**：任何人 clone 后按 README 一键构建；CI 全绿；发布 v0.1.0。

---

### 阶段 8：Linux 迁移（全程并行）

**目标**：开发环境从 Windows 渐进迁移到 Linux（WSL2 起步），养成跨平台意识。

**核心概念**：bash 命令、文件系统/权限、包管理 apt、Linux 上装 gcc/clang/cmake/conan、gdb/valgrind。

**资料**：[知识库 09：Linux 开发环境](C++学习资料/09-Linux开发环境.md) + Linux Journey（linuxjourney.com）+ TLCL。

**项目作业**：
- 装 WSL2 + Ubuntu，装工具链；
- 在 Linux 上构建 ThirdLink（cmake + conan + ninja）；
- CI 加 ubuntu 平台；日常开发逐步切到 WSL2。

**验收**：Linux 上一键构建；能用 bash + nvim 日常开发。

---

### 阶段 6：回嵌入式（未来路线，现在只做铺垫）

**目标**：把纯 C++ 里建立的结构直觉，带回 C 的嵌入式世界。

**要点**：
- 嵌入式没有 STL → 用 C 实现侵入式链表、环形缓冲、内存池；
- 链表在主机端罕见、在嵌入式是主力；
- 到时候你写的是「知道为什么」的实现，而不是抄模板。

---

## 五、书单与资源

**两本核心书**：
1. 《A Tour of C++》第三版 / 中译《C++ 语言导学》——总览教材，按阶段配套读；
2. 《Effective Modern C++》/ 中译同名——设计哲学，什么时候用什么、为什么。

**免费在线**：
- cppreference.com（词典，查语法）；
- C++ Core Guidelines（社区公认设计准则）；
- learncpp.com（带练习的教程）；
- 《Hello 算法》hello-algo.com（数据结构动画教材）；
- Linux Journey（linuxjourney.com，Linux 入门免费教材）；
- 《The Linux Command Line》TLCL（免费在线，Linux 命令经典）。

**备选**：《C++ Primer 5th》（大部头教科书）、《算法图解》（数据结构入门读物）、《鸟哥的 Linux 私房菜》（中文 Linux 经典）。

---

## 六、设计观念速记

- **工具优先**：先会用工具，再补理论，理论指导造轮子；
- **YAGNI**：需求出现才引入特性——这是你的学习哲学，也是 C++ 社区主流；
- **默认 vector**：除非有理由（Core Guidelines SL.con.2）；
- **接口用 span**：函数参数只读时用 std::span，不关心来源；
- **RAII**：资源随对象生命周期管理，杜绝手动配对；
- 完整对照表见 [知识库 06](C++学习资料/06-设计观念与参考.md)。

---

（完）
