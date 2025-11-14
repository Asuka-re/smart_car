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
    Executor eN({0, 0, 'N'});  eN.Execute("M");
    EXPECT_EQ(eN.Query().y, 1);

    Executor eE({0, 0, 'E'});  eE.Execute("M");
    EXPECT_EQ(eE.Query().x, 1);

    Executor eS({0, 0, 'S'});  eS.Execute("M");
    EXPECT_EQ(eS.Query().y, -1);

    Executor eW({0, 0, 'W'});  eW.Execute("M");
    EXPECT_EQ(eW.Query().x, -1);
}

TEST(AccelTest, AccelMove) {
    Executor e({0, 0, 'N'});
    e.Execute("F"); e.Execute("M");        // 加速前进 2 格
    EXPECT_EQ(e.Query().y, 2);
}

TEST(ReverseTest, BasicBack) {
    Executor e({0, 0, 'N'});
    e.Execute("B");   // 进入倒车
    e.Execute("M");   // 后退一格
    auto p = e.Query();
    EXPECT_EQ(p.x, 0);
    EXPECT_EQ(p.y, -1);
    EXPECT_EQ(p.heading, 'N');   
}

TEST(ReverseTest, BackWithAccel) {
    Executor e({0, 0, 'N'});
    e.Execute("B"); e.Execute("F"); e.Execute("M"); // 倒车+加速
    EXPECT_EQ(e.Query().y, -2);
}

TEST(ReverseTest, BackTurnCombo) {
    Executor e({0, 0, 'E'});
    e.Execute("B"); e.Execute("F"); e.Execute("L"); // 退1+左转
    auto p = e.Query();
    EXPECT_EQ(p.x, -1);
    EXPECT_EQ(p.heading, 'N');
}

TEST(ReverseTest, ToggleBackMode) {
    Executor e({0, 0, 'S'});
    e.Execute("B"); e.Execute("B");   // 再按一次取消
    e.Execute("M");                   // 正常前进
    EXPECT_EQ(e.Query().y, -1);       // 原朝向S，前进 y-1
    EXPECT_EQ(e.Query().heading, 'S');
}

TEST(TurnTest, LeftOnly) {
    Executor e({0, 0, 'N'});
    e.Execute("L");
    EXPECT_EQ(e.Query().heading, 'W');
}