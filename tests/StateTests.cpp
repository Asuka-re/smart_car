#include <gtest/gtest.h>
#include <memory>
#include "executor.h"
using namespace adas;

// 状态与切换 / Reverse+Fast 方向测试 (ID:17,18,19,20,21,22,26,27)

// ID:17 Reverse R 后退再左转(东->北)
TEST(StateTests, reverse_R_E_returns_minus_1_then_N) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0,0,'E'}));
    e->Execute("BR");
    EXPECT_EQ((Pose{-1,0,'N'}), e->Query());
}

// ID:18 BF R 后退两格再左转(东->北)
TEST(StateTests, bf_R_E_returns_minus_2_then_N) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0,0,'E'}));
    e->Execute("BFR");
    EXPECT_EQ((Pose{-2,0,'N'}), e->Query());
}

// ID:19 BF 激活后 M 后退2 再 F 关闭Fast
TEST(StateTests, seq_BF_M_F_state_toggle_fast_off) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor());
    e->Execute("BF M F");
    EXPECT_EQ((Pose{0,-2,'N'}), e->Query());
}

// ID:20 Fast W 朝向双步前进
TEST(StateTests, fast_M_W_returns_x_minus_2) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0,0,'W'}));
    e->Execute("FM");
    EXPECT_EQ((Pose{-2,0,'W'}), e->Query());
}

// ID:21 Reverse N 后退 y-1
TEST(StateTests, reverse_M_N_returns_y_minus_1) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor());
    e->Execute("BM");
    EXPECT_EQ((Pose{0,-1,'N'}), e->Query());
}

// ID:22 BF N 后退 y-2
TEST(StateTests, bf_M_N_returns_y_minus_2) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor());
    e->Execute("BFM");
    EXPECT_EQ((Pose{0,-2,'N'}), e->Query());
}

// ID:26 Fast 双切换关闭
TEST(StateTests, fast_toggle_off_sequence_F_F_M) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor());
    e->Execute("F F M");
    EXPECT_EQ((Pose{0,1,'N'}), e->Query());
}

// ID:27 Reverse+Fast 快速切换最终仅 Reverse
TEST(StateTests, reverse_fast_toggle_sequence_B_F_F_M) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor());
    e->Execute("B F F M");
    EXPECT_EQ((Pose{0,-1,'N'}), e->Query());
}
