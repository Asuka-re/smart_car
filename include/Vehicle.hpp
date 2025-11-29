#pragma once
#include "PoseHandler.hpp"

namespace adas {
// 预留的车辆策略接口：未来可为不同车型定制加速倍率、转向前置位移等。
class Vehicle {
public:
    virtual ~Vehicle() = default;
    virtual const char* Name() const noexcept = 0;
    // 针对移动的步长倍率（Fast 状态外的基础倍率，可与 Fast 叠加）
    virtual int BaseMoveMultiplier() const noexcept { return 1; }
    // Reverse 下转向前是否需要附加位移（已在 Command 层实现基础逻辑，可扩展附加规则）
    virtual int ExtraReverseTurnBackwardSteps(bool fast) const noexcept {
        return fast ? 2 : 1; // 默认与当前实现保持一致
    }
};

class BasicVehicle final : public Vehicle {
public:
    const char* Name() const noexcept override { return "Basic"; }
};

class BusVehicle final : public Vehicle {
public:
    // 假设 Bus 移动步长较小（例如重车）
    int BaseMoveMultiplier() const noexcept override { return 1; }
    const char* Name() const noexcept override { return "Bus"; }
};

class SportsCarVehicle final : public Vehicle {
public:
    // 假设 SportsCar 在基础移动上有更大加速潜力
    int BaseMoveMultiplier() const noexcept override { return 2; }
    const char* Name() const noexcept override { return "SportsCar"; }
};

} // namespace adas
