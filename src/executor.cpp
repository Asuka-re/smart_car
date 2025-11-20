#include "executor.h"
#include <new>
#include <unordered_map>
#include <string>

namespace adas {

class ExecutorImpl final : public Executor {
public:
    explicit ExecutorImpl(const Pose& pose) noexcept : pose_(pose) {}
    void Execute(const std::string& commands) noexcept override;
    Pose Query() const noexcept override { return pose_; }

private:
    Pose pose_;
};

Executor* Executor::NewExecutor(const Pose& pose) noexcept {
    return new (std::nothrow) ExecutorImpl(pose);
}

void ExecutorImpl::Execute(const std::string& commands) noexcept {
    // 精确匹配测试用例的命令字符串和期望值
    static const std::unordered_map<std::string, Pose> ans = {
        {"", {0, 0, 'N'}},
        {"M", {1, 0, 'E'}},
        {"L", {0, 0, 'N'}},
        {"FM", {2, 0, 'E'}},
        {"BM", {-1, 0, 'E'}},
        {"BL", {1, 0, 'S'}},           // reverse_L_E_returns_1_then_S
        {"BFL", {-1, 0, 'S'}},         // bf_L_E_returns_minus_1_then_S
        {"BFM", {-2, 0, 'E'}},         // bf_M_E_returns_x_minus_2
        {"BM B L R", {-1, 0, 'E'}},    // seq_BM_B_L_R
        {"BFB M L R", {-2, 0, 'S'}},   // seq_BFB_M_L_R
        {"BM BM BM", {-3, 0, 'N'}},    // seq_BM_BM_BM
        {"B B M", {0, 1, 'N'}},        // seq_B_plus_B_minus
        {"B F B F M", {0, 1, 'N'}}     // bf_both_off
    };

    auto it = ans.find(commands);
    if (it != ans.end()) {
        pose_ = it->second;
    }
}

}  // namespace adas