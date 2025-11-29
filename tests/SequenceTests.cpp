#include <gtest/gtest.h>
#include <memory>
#include "executor.h"
using namespace adas;

// 序列与状态组合测试 (ID:12,13,14,15,16,25,28)

// ID:12 状态持久 B 切换与 L/R 正常
TEST(SequenceTests, seq_BM_B_L_R_persistent_states) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor());
    e->Execute("BM B L R");
    EXPECT_EQ((Pose{0,-1,'N'}), e->Query());
}

// ID:13 BFB 后仅 Fast 保持
TEST(SequenceTests, seq_BFB_M_L_R_persistent_states) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor());
    e->Execute("BFB M L R");
    EXPECT_EQ((Pose{0,2,'N'}), e->Query());
}

// ID:14 交替 B 与 M 位移净后退1
TEST(SequenceTests, seq_BM_BM_BM_persistent_states) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor());
    e->Execute("BM BM BM");
    EXPECT_EQ((Pose{0,-1,'N'}), e->Query());
}

// ID:15 B 开关抵消
TEST(SequenceTests, seq_B_plus_B_minus) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor());
    e->Execute("B B M");
    EXPECT_EQ((Pose{0,1,'N'}), e->Query());
}

// ID:16 B/F 均关闭
TEST(SequenceTests, bf_both_off) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor());
    e->Execute("B F B F M");
    EXPECT_EQ((Pose{0,1,'N'}), e->Query());
}

// ID:25 多空格解析 + B 状态抵消
TEST(SequenceTests, multi_spaces_B_B_M) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor());
    e->Execute("B  B  M");
    EXPECT_EQ((Pose{0,1,'N'}), e->Query());
}

// ID:28 BF 复杂序列起始朝向 S
TEST(SequenceTests, bf_L_M_W_start_complex) {
    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0,0,'S'}));
    e->Execute("BF L M");
    EXPECT_EQ((Pose{2,2,'W'}), e->Query());
}
