# Smart-Car Executor

高内聚、可扩展的指令执行框架：将解析、状态、动作分层。支持移动 (M)、左/右转 (L/R)、加速 (F)、倒车 (B)、掉头 (TR) 等指令；状态 (Fast/Reverse) 跨组持久。

## 架构分层
| 层 | 说明 |
| --- | --- |
| PoseHandler | 管理坐标与朝向及 Fast / Reverse 状态标志 |
| ActionGroup | 原子动作序列 (Forward/Backward/Turn...) 聚合执行 |
| Command | 将当前状态映射为 ActionGroup（含 Reverse 下转向前后退逻辑） |
| CmderFactory | 解析字符串到命令对象列表；处理 `TR` -> `Z` 替换与状态字符 F/B 持久切换 |
| Executor | 协调执行：解析 -> ActionGroup -> PoseHandler |

## 指令与状态语义速览
1. 状态持久 (A)：F/B 一经出现开启或关闭 Fast/Reverse，跨空格分组保持，直到再次出现同字符翻转。
2. Reverse 转向前移动 (B)：Reverse 下每个 L/R 在实际转向前后退 1 格；若同时 Fast 则后退 2 格。
3. Reverse 方向互换 (C)：Reverse 状态中 L 执行右转，R 执行左转（通过 REVERSE_TURN* 映射实现）。
4. 状态跨组保持 (D)：空格仅用于视觉分组，不触发状态重置。
5. TR（掉头）：Normal 模式：`L F L` 序列；Fast 模式：`F L F L`；Reverse 下被忽略（无动作）。

## 测试矩阵 (GoogleTest)
共 28 条用例，分组如下：
- 基础: 初始姿态、前进、左转、Fast 前进、Reverse / BF 前进与转向 (ID 4~11)
- 状态与序列切换: B/F 开关、交替与持久、空格解析 (ID 12~16, 19, 25~27)
- Reverse/Fast 组合方向: 不同初始朝向与复杂序列 (ID 17,18,20~22,28)
- TR 行为与交互: 正常、Fast、Reverse 忽略以及后续动作 (ID 1~3, 23,24)

所有测试通过：`ctest --output-on-failure`。

## 构建
使用 CMake + FetchContent 集成 GoogleTest：
```bash
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

## 扩展点：Vehicle（预留）
将引入 `Vehicle` 抽象以支持不同车型策略（例如 Bus 转向半径或 SportsCar 加速倍率）。当前版本未改变语义，只在设计上预留。

## 目录结构要点
```
include/        头文件 (ActionGroup, Command, CmderFactory, PoseHandler, Singleton)
src/            实现文件
tests/          GoogleTest 用例（分组文件）
third_party/    googletest 源码 (FetchContent)
```

## 运行示例
```bash
./smartcar_main            # 生成的示例主程序
```

## 快速验证指令
在 `main.cpp` 中修改：
```cpp
executor->Execute("BFM L R TR");
```

## 质量策略
- 编译标志：`-Wall -Wextra -Werror`
- C++17，静态库封装核心逻辑
- 状态与行为单元测试完备（28/28 PASS）

## 后续计划
1. 引入 Vehicle 策略接口与多车型单元测试
2. 覆盖率统计与 CI 集成（gcov/llvm-cov）
3. 性能 Profiling（大规模命令串解析）

欢迎继续扩展或提出新场景！
