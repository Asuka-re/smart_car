#include <gtest/gtest.h>
#include <memory>
#include "executor.h"
using namespace adas;

// 基础与状态基础测试 (ID:4~11)

// ID:4 默认姿态
TEST(BaseTests, should_return_default_pose_when_without_init_and_command) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor());
    EXPECT_EQ((Pose{0,0,'N'}), e->Query());
}

// ID:5 前进一格(东)
TEST(BaseTests, should_return_x_plus_1_given_command_is_M_and_facing_is_E) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0,0,'E'}));
    e->Execute("M");
    EXPECT_EQ((Pose{1,0,'E'}), e->Query());
}

// ID:6 左转(东->北)
TEST(BaseTests, should_return_facing_N_given_command_is_L_and_facing_is_E) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0,0,'E'}));
    e->Execute("L");
    EXPECT_EQ('N', e->Query().heading);
}

// ID:7 Fast 前进双倍(东)
TEST(BaseTests, accel_M_E_returns_x_plus_2) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0,0,'E'}));
    e->Execute("FM");
    EXPECT_EQ((Pose{2,0,'E'}), e->Query());
}

// ID:8 Reverse M 后退一格(东)
TEST(BaseTests, reverse_M_E_returns_x_minus_1) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0,0,'E'}));
    e->Execute("BM");
    EXPECT_EQ((Pose{-1,0,'E'}), e->Query());
}

// ID:9 Reverse L 后退再右转(东->南)
TEST(BaseTests, reverse_L_E_returns_minus_1_then_S) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0,0,'E'}));
    e->Execute("BL");
    EXPECT_EQ((Pose{-1,0,'S'}), e->Query());
}

// ID:10 BF M 后退两格(东)
TEST(BaseTests, bf_M_E_returns_x_minus_2) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0,0,'E'}));
    e->Execute("BFM");
    EXPECT_EQ((Pose{-2,0,'E'}), e->Query());
}

// ID:11 BF L 后退两格再右转(东->南)
TEST(BaseTests, bf_L_E_returns_minus_2_then_S) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0,0,'E'}));
    e->Execute("BFL");
    EXPECT_EQ((Pose{-2,0,'S'}), e->Query());
}
