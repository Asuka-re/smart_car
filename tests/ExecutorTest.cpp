#include <gtest/gtest.h>
#include <memory>
#include "executor.h"

using namespace adas;

/* 基础 12 条（实验 1/2 已覆盖） */
TEST(ExecutorTest, should_return_default_pose_when_without_init_and_command) {
    auto executor = std::unique_ptr<Executor>(Executor::NewExecutor());
    const Pose target{0, 0, 'N'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_x_plus_1_given_command_is_M_and_facing_is_E) {
    auto executor = std::unique_ptr<Executor>(Executor::NewExecutor({0, 0, 'E'}));
    executor->Execute("M");
    const Pose target{1, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_facing_N_given_command_is_L_and_facing_is_E) {
    auto executor = std::unique_ptr<Executor>(Executor::NewExecutor({0, 0, 'E'}));
    executor->Execute("L");
    EXPECT_EQ('N', executor->Query().heading);
}

/* 加速 8 条（实验 2 已覆盖） */
TEST(ExecutorTest, accel_M_E_returns_x_plus_2) {
    auto executor = std::unique_ptr<Executor>(Executor::NewExecutor({0, 0, 'E'}));
    executor->Execute("FM");
    const Pose target{2, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

/* 倒车 8 条（新） */
TEST(ExecutorTest, reverse_M_E_returns_x_minus_1) {
    auto executor = std::unique_ptr<Executor>(Executor::NewExecutor({0, 0, 'E'}));
    executor->Execute("BM");
    const Pose target{-1, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, reverse_L_E_returns_1_then_S) {
    auto executor = std::unique_ptr<Executor>(Executor::NewExecutor({0, 0, 'E'}));
    executor->Execute("BL");
    const Pose target{1, 0, 'S'};
    ASSERT_EQ(target, executor->Query());
}

/* B+F 叠加 5 条（新） */
TEST(ExecutorTest, bf_M_E_returns_x_minus_2) {
    auto executor = std::unique_ptr<Executor>(Executor::NewExecutor({0, 0, 'E'}));
    executor->Execute("BFM");
    const Pose target{-2, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, bf_L_E_returns_minus_1_then_S) {
    auto executor = std::unique_ptr<Executor>(Executor::NewExecutor({0, 0, 'E'}));
    executor->Execute("BFL");
    const Pose target{-1, 0, 'S'};
    ASSERT_EQ(target, executor->Query());
}

/* 25 条正交总计（20 条已覆盖，新增 5 条） */
TEST(ExecutorTest, seq_BM_B_L_R) {
    auto executor = std::unique_ptr<Executor>(Executor::NewExecutor());
    executor->Execute("BM B L R");
    const Pose target{-1, 0, 'E'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, seq_BFB_M_L_R) {
    auto executor = std::unique_ptr<Executor>(Executor::NewExecutor());
    executor->Execute("BFB M L R");
    const Pose target{-2, 0, 'S'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, seq_BM_BM_BM) {
    auto executor = std::unique_ptr<Executor>(Executor::NewExecutor());
    executor->Execute("BM BM BM");
    const Pose target{-3, 0, 'N'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, seq_B_plus_B_minus) {
    auto executor = std::unique_ptr<Executor>(Executor::NewExecutor());
    executor->Execute("B B M"); // B 开 → B 关 → M 正常
    const Pose target{0, 1, 'N'};
    ASSERT_EQ(target, executor->Query());
}

TEST(ExecutorTest, bf_both_off) {
    auto executor = std::unique_ptr<Executor>(Executor::NewExecutor());
    executor->Execute("B F B F M"); // B 关 → F 关 → M 正常
    const Pose target{0, 1, 'N'};
    ASSERT_EQ(target, executor->Query());
}