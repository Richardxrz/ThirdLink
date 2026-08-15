# 知识库 07：ThirdLink 改造清单

> 归属阶段：全程。代码层 / 构建层 / 工程化的改造任务清单（含进度）。

代码层：
    [x] 删死代码：InitJoint、InitLink、DrawJoint、DrawLink、EndEffector、size 变量 —— 2026-08-13 已验证无残留
    [x] 删冗余：joints[]、links[] 数组（数据只用一份）—— 已用 std::vector<Link>
    [x] typedef struct → struct；#define → constexpr；复合字面量 → C++ 聚合初始化 —— 已完成
    [ ] 文件内函数改 static / 匿名命名空间（去掉无意义的 inline）—— 待做（DrawLink/DrawArm/PrintInitInfo）
    [x] Arm 裸指针+计数 → std::vector<Link> —— 已完成；DrawArm 保持 const Arm&（内部函数，YAGNI 不换 span）
    [ ] Segment/Arm 类化；模型层与渲染层分离 —— 阶段 1 任务，待做
    [ ] InitWindow/CloseWindow 用 RAII 包装 —— 阶段 1 任务，待做

构建层：
    [x] CMakePresets：Libr → Lib、gcc-release → Release、cmake_minimum_required 3.21 —— 已完成
    [x] toolchains 拼写（CMKAE_OBJCOPY 等）—— 已验证无拼写错误
    [x] 统一加 -Wall -Wextra -Wpedantic（MSVC /W4）—— 已完成，验证 0 警告

工程化：
    [x] git init + .gitignore + README —— 已完成
    [x] 接 Conan（替换硬编码路径）—— 已完成，Debug + Release 双配置跑通
    [x] .gitattributes（换行符策略）—— 已完成
    [x] GitHub Actions 三平台 CI —— 文件已写好，待推送验证
    [ ] 后续：运动学库、矩阵库（参考 GLM）、重力环境（参考 Box2D）、单元测试 —— 阶段 3 任务，待做

---
