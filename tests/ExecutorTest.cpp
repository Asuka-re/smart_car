#include <gtest/gtest.h>
#include "executor.h"

using namespace adas;

TEST(InitTest, DefaultInit) {
    Executor e;
    auto p = e.Query();
    EXPECT_EQ(p.x, 0);
    EXPECT_EQ(p.y, 0);
    EXPECT_EQ(p.heading, 'N');
}

TEST(MoveTest, MoveDirections) {
    Executor eN({0,0,'N'});
    eN.Execute("M");
    auto p = eN.Query();
    EXPECT_EQ(p.x, 0);
    EXPECT_EQ(p.y, 1);
    EXPECT_EQ(p.heading, 'N');

    Executor eE({0,0,'E'});
    eE.Execute("M");
    p = eE.Query();
    EXPECT_EQ(p.x, 1);
    EXPECT_EQ(p.y, 0);
    EXPECT_EQ(p.heading, 'E');

    Executor eS({0,0,'S'});
    eS.Execute("M");
    p = eS.Query();
    EXPECT_EQ(p.x, 0);
    EXPECT_EQ(p.y, -1);
    EXPECT_EQ(p.heading, 'S');

    Executor eW({0,0,'W'});
    eW.Execute("M");
    p = eW.Query();
    EXPECT_EQ(p.x, -1);
    EXPECT_EQ(p.y, 0);
    EXPECT_EQ(p.heading, 'W');
}

TEST(TurnTest, LAndRNoMove) {
    Executor e({0,0,'N'});
    e.Execute("L");
    auto p = e.Query();
    EXPECT_EQ(p.heading, 'W');
    EXPECT_EQ(p.x, 0);
    EXPECT_EQ(p.y, 0);

    e.Execute("R");
    p = e.Query();
    EXPECT_EQ(p.heading, 'N');
}

TEST(AccelTest, MAndTurnsWithAccel) {
    Executor e({0,0,'N'});
    e.Execute("F");    // accel on
    e.Execute("M");    // move 2 north
    auto p = e.Query();
    EXPECT_EQ(p.x, 0);
    EXPECT_EQ(p.y, 2);
    EXPECT_EQ(p.heading, 'N');

    e.Execute("L");    // accel on: move1 then left => y=3 heading=W
    p = e.Query();
    EXPECT_EQ(p.x, 0);
    EXPECT_EQ(p.y, 3);
    EXPECT_EQ(p.heading, 'W');

    e.Execute("F");    // accel off
    e.Execute("M");    // normal move west => x=-1
    p = e.Query();
    EXPECT_EQ(p.x, -1);
    EXPECT_EQ(p.y, 3);
    EXPECT_EQ(p.heading, 'W');
}

TEST(SequenceTest, MixedCommands) {
    Executor e({1,2,'E'});
    e.Execute("MFRMLM"); // exercise mixed commands
    auto p = e.Query();
    EXPECT_TRUE(p.x >= INT32_MIN && p.x <= INT32_MAX);
    EXPECT_TRUE(p.y >= INT32_MIN && p.y <= INT32_MAX);
    EXPECT_TRUE(p.heading == 'N' || p.heading == 'S' || p.heading == 'E' || p.heading == 'W');
}