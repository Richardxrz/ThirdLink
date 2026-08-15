# 知识库 10：ThirdLink 项目规划

> 归属阶段：全程。项目的定位、路线图、架构与开源计划。

## 一、项目定位（一句话）

「一个以学习工程化为首要目标的 2D 机械臂运动学/动力学仿真练习项目」

- **首要目标**：完整走一遍工程化流程（版本控制/构建/依赖/测试/CI/文档/发布/开源礼仪），
  养成工程化习惯与意识——功能"够用即可"；
- **不追求**：与 matlab / rviz / moveit 竞争——它们是成熟产品，你是学习载体；
- **参考**：matlab robotics toolbox / rviz / moveit 的 API 设计思路，功能上不比较。

## 二、愿景演进（未定型，走一步看一步）

    第 1 阶段：机械臂运动学仿真器（当前）
    第 2 阶段：2D 物理/渲染引擎雏形（向量/矩阵库、物理模拟、场景管理）
    第 3 阶段：游戏引擎前身（可选，随缘）

这决定了技术选型方向：**核心逻辑与渲染解耦**（模型/渲染分层），
未来渲染后端可替换（raylib → 其他），核心库可复用。

## 三、功能路线图（里程碑）

    M0 静态三连杆绘制（已完成）
    M1 交互：鼠标拖拽末端 / 角度滑条调节
    M2 正向运动学 API 完善（模型/渲染分离，FK 可测试）
    M3 反向运动学（解析法两连杆 / 迭代法多连杆）
    M4 简单动力学：重力、质点、欧拉积分
    M5 轨迹规划：关节空间插值 / 笛卡尔直线
    M6 A* 避障（网格场景）
    M7 渲染后端抽象（raylib 可替换接口）
    M8 开源发布（文档/CI/Release 齐全）

## 四、工程化路线（与功能并行，首要目标）

    E0 git init + README + License（做功能前先做）
    E1 CMake 规范 + Conan 依赖管理
    E2 单元测试框架（Catch2/GoogleTest）+ CTest
    E3 GitHub Actions CI（三平台矩阵）
    E4 Doxygen API 文档
    E5 SemVer + CHANGELOG + GitHub Release
    E6 CONTRIBUTING + Issue/PR 模板（社区流程）

## 五、目录结构建议

    thirdlink/
    ├── include/thirdlink/    公共头文件（API 面）
    ├── src/                  实现
    ├── tests/                单元测试
    ├── examples/             示例
    ├── docs/                 文档
    ├── CMakeLists.txt
    ├── README.md
    ├── LICENSE
    └── .github/              CI + issue/PR 模板

## 六、API 设计原则（你首要目标之一）

- 公共头文件只暴露稳定接口，实现细节藏进 src/；
- 命名一致：类名 PascalCase、函数 camelCase、常量 k 前缀；
- const 正确性：能 const 就 const；
- 参数用 span 接收连续数据；
- 头文件自包含：每个头文件能独立编译；
- Doxygen 注释公共 API。

## 七、开源发布检查清单（发布前逐项打勾）

    [ ] LICENSE 文件
    [ ] README：项目名/一句话/截图/构建命令/使用示例
    [ ] .gitignore：build/、.cache/
    [ ] CI 绿：三平台编译 + 测试通过
    [ ] 编译 0 警告
    [ ] CHANGELOG
    [ ] git tag + GitHub Release
    [ ] CONTRIBUTING + CODE_OF_CONDUCT（可选）
