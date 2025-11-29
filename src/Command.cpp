#include "Command.hpp"

namespace adas {
// Command 实现要点：
//  - M: Fast 下双步（通过重复同向动作），Reverse 下改为后退。
//  - L/R: Normal 直接转向；Reverse 下先后退 (Fast 时双倍)，再执行反向转向（左右互换）。
//  - TR(Z): Reverse 状态忽略；Fast 下插入双前进；Normal 下为 L F L。
// 语义规则参考 README 中 A-D。
ActionGroup MoveCommand::operator()(PoseHandler& poseHandler) const noexcept {
    ActionGroup group;
    const auto action = poseHandler.IsReverse() ? ActionType::BACKWARD_1_STEP_ACTION 
                                                : ActionType::FORWARD_1_STEP_ACTION;
    if (poseHandler.IsFast()) group.PushAction(action);
    group.PushAction(action);
    return group;
}

ActionGroup TurnLeftCommand::operator()(PoseHandler& poseHandler) const noexcept {
    ActionGroup group;
    // 语义更新：
    //  A. Fast/Reverse 状态跨组持久，不自动复位（复位逻辑已从工厂移除）
    //  B. Reverse 状态下，任一转向(L/R)在执行转向前需要“先后退”一格；若同时 Fast 则后退距离翻倍（两次后退动作）
    //  C. Reverse 状态下左右转向互换：L 实际执行右转；R 实际执行左转（通过 REVERSE_* 枚举映射实现）
    //  D. Fast/Reverse 仅在再次收到 F/B 指令时才切换，跨空格分组保持。
    if (poseHandler.IsReverse()) {
        // 后退 1 格；若 Fast 再补一次后退实现翻倍
        group.PushAction(ActionType::BACKWARD_1_STEP_ACTION);
        if (poseHandler.IsFast()) group.PushAction(ActionType::BACKWARD_1_STEP_ACTION);
        group.PushAction(ActionType::REVERSE_TURNLEFT_ACTION); // 映射为实际右转
    } else {
        // 正常模式不预移动，只执行左转
        group.PushAction(ActionType::TURNLEFT_ACTION);
    }
    return group;
}

ActionGroup TurnRightCommand::operator()(PoseHandler& poseHandler) const noexcept {
    ActionGroup group;
    if (poseHandler.IsReverse()) {
        group.PushAction(ActionType::BACKWARD_1_STEP_ACTION);
        if (poseHandler.IsFast()) group.PushAction(ActionType::BACKWARD_1_STEP_ACTION);
        group.PushAction(ActionType::REVERSE_TURNRIGHT_ACTION); // 映射为实际左转
    } else {
        group.PushAction(ActionType::TURNRIGHT_ACTION);
    }
    return group;
}

ActionGroup FastCommand::operator()(PoseHandler& poseHandler) const noexcept {
    (void)poseHandler; // silence unused parameter under -Werror
    return ActionGroup({ActionType::BE_FAST_ACTION});
}

ActionGroup ReverseCommand::operator()(PoseHandler& poseHandler) const noexcept {
    (void)poseHandler;
    return ActionGroup({ActionType::BE_REVERSE_ACTION});
}

ActionGroup TurnRoundCommand::operator()(PoseHandler& poseHandler) const noexcept {
    if (poseHandler.IsReverse()) return ActionGroup();
    if (poseHandler.IsFast()) {
        return ActionGroup({ActionType::FORWARD_1_STEP_ACTION, ActionType::TURNLEFT_ACTION,
                           ActionType::FORWARD_1_STEP_ACTION, ActionType::TURNLEFT_ACTION});
    }
    return ActionGroup({ActionType::TURNLEFT_ACTION, ActionType::FORWARD_1_STEP_ACTION, ActionType::TURNLEFT_ACTION});
}
}  // namespace adas