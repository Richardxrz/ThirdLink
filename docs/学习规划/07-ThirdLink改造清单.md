# 知识库 07：ThirdLink 改造清单

> 归属阶段：全程。代码层 / 构建层 / 工程化的改造任务清单。

代码层：
    [ ] 删死代码：InitJoint、InitLink、DrawJoint、DrawLink、EndEffector、size 变量
    [ ] 删冗余：joints[]、links[] 数组（数据只用一份）
    [ ] typedef struct → struct；#define 常量 → constexpr；复合字面量 → C++ 聚合初始化
    [ ] 文件内函数改 static / 匿名命名空间（去掉无意义的 inline）
    [ ] Arm 的裸指针+计数 → std::vector<Segment>；DrawArm 收 std::span
    [ ] Segment/Arm 类化；模型层与渲染层分离
    [ ] InitWindow/CloseWindow 用 RAII 包装

构建层：
    [ ] CMakePresets：Libr → Lib；gcc-release → Release；cmake_minimum_required 降到 3.20
    [ ] toolchains/*.cmake：CMKAE_OBJCOPY → CMAKE_OBJCOPY 等拼写修复；删 CMAKE_GENERATOR
    [ ] 统一加 -Wall -Wextra -Wpedantic（MSVC /W4）

工程化：
    [ ] git init + .gitignore + README
    [ ] 接 Conan（第6节步骤），替换硬编码路径
    [ ] 后续：运动学库、矩阵库（参考 GLM）、重力环境（参考 Box2D）、单元测试

---

