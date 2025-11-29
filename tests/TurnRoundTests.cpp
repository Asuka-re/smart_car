#include <gtest/gtest.h>
#include <memory>
#include "executor.h"
using namespace adas;

// 掉头与其交互测试 (ID:1,2,3,23,24)

// ID:1 Normal TR: L F L
TEST(TurnRoundTests, should_normal_tr_build_left_forward_left) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0,0,'E'}));
    e->Execute("TR");
    EXPECT_EQ((Pose{0,1,'W'}), e->Query());
}

// ID:2 Fast TR: F L F L
TEST(TurnRoundTests, should_fast_tr_build_forward_left_forward_left) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0,0,'E'}));
    e->Execute("FTR");
    EXPECT_EQ((Pose{1,1,'W'}), e->Query());
}

// ID:3 Reverse 下 TR 忽略
TEST(TurnRoundTests, in_the_B_state_the_reverse_command_will_be_ignored) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0,0,'E'}));
    e->Execute("BTR");
    EXPECT_EQ((Pose{0,0,'E'}), e->Query());
}

// ID:23 Fast 后接 TR 再 M 双步
TEST(TurnRoundTests, fast_TR_then_M) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0,0,'E'}));
    e->Execute("FTR M");
    EXPECT_EQ((Pose{-1,1,'W'}), e->Query());
}

// ID:24 FB 下 TR 忽略 后 M 后退2
TEST(TurnRoundTests, fb_TR_ignored_then_M) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0,0,'E'}));
    e->Execute("FBTR M");
    EXPECT_EQ((Pose{-2,0,'E'}), e->Query());
}
