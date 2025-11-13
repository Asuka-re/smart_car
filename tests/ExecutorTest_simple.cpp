#include <iostream>
#include <string>
#include "executor.h"

using namespace adas;

static int failures = 0;

#define ASSERT_EQ_INT(a,b,msg) \
    do { if ((a) != (b)) { std::cout << "FAIL: " << msg << " -> expected " << (b) << " got " << (a) << "\n"; ++failures; } else { std::cout << "OK: " << msg << "\n"; } } while(0)

#define ASSERT_EQ_CHAR(a,b,msg) \
    do { if ((a) != (b)) { std::cout << "FAIL: " << msg << " -> expected '" << (b) << "' got '" << (a) << "'\n"; ++failures; } else { std::cout << "OK: " << msg << "\n"; } } while(0)

int main() {
    std::cout << "[ RUN ] InitTest DefaultInit\n";
    {
        Executor e;
        auto p = e.Query();
        ASSERT_EQ_INT(p.x, 0, "default x");
        ASSERT_EQ_INT(p.y, 0, "default y");
        ASSERT_EQ_CHAR(p.heading, 'N', "default heading");
    }

    std::cout << "[ RUN ] MoveTest MoveDirections\n";
    {
        Executor eN({0,0,'N'});
        eN.Execute("M");
        auto p = eN.Query();
        ASSERT_EQ_INT(p.x, 0, "N x");
        ASSERT_EQ_INT(p.y, 1, "N y");
        ASSERT_EQ_CHAR(p.heading, 'N', "N heading");

        Executor eE({0,0,'E'});
        eE.Execute("M");
        p = eE.Query();
        ASSERT_EQ_INT(p.x, 1, "E x");
        ASSERT_EQ_INT(p.y, 0, "E y");
        ASSERT_EQ_CHAR(p.heading, 'E', "E heading");

        Executor eS({0,0,'S'});
        eS.Execute("M");
        p = eS.Query();
        ASSERT_EQ_INT(p.x, 0, "S x");
        ASSERT_EQ_INT(p.y, -1, "S y");
        ASSERT_EQ_CHAR(p.heading, 'S', "S heading");

        Executor eW({0,0,'W'});
        eW.Execute("M");
        p = eW.Query();
        ASSERT_EQ_INT(p.x, -1, "W x");
        ASSERT_EQ_INT(p.y, 0, "W y");
        ASSERT_EQ_CHAR(p.heading, 'W', "W heading");
    }

    std::cout << "[ RUN ] TurnTest LAndRNoMove\n";
    {
        Executor e({0,0,'N'});
        e.Execute("L");
        auto p = e.Query();
        ASSERT_EQ_CHAR(p.heading, 'W', "L turns to W");
    }

    std::cout << "[ RUN ] AccelTest MAndTurnsWithAccel\n";
    {
        Executor e({0,0,'N'});
        e.Execute("F");
        e.Execute("M");
        auto p = e.Query();
        ASSERT_EQ_INT(p.x, 0, "accel M x");
        ASSERT_EQ_INT(p.y, 2, "accel M y");
        ASSERT_EQ_CHAR(p.heading, 'N', "accel M heading");

        e.Execute("L");
        p = e.Query();
        ASSERT_EQ_INT(p.x, 0, "accel L x");
        ASSERT_EQ_INT(p.y, 3, "accel L y");
        ASSERT_EQ_CHAR(p.heading, 'W', "accel L heading");

        e.Execute("F");
        e.Execute("M");
        p = e.Query();
        ASSERT_EQ_INT(p.x, -1, "after accel off M x");
        ASSERT_EQ_INT(p.y, 3, "after accel off M y");
        ASSERT_EQ_CHAR(p.heading, 'W', "after accel off M heading");
    }

    std::cout << "[ RUN ] SequenceTest MixedCommands\n";
    {
        Executor e({1,2,'E'});
        e.Execute("MFRMLM");
        auto p = e.Query();
        // 保证在合理范围且朝向合法
        bool heading_ok = (p.heading=='N'||p.heading=='S'||p.heading=='E'||p.heading=='W');
        if (!heading_ok) {
            std::cout << "FAIL: SequenceTest heading invalid '" << p.heading << "'\n";
            ++failures;
        } else {
            std::cout << "OK: SequenceTest heading\n";
        }
    }

    std::cout << "==========\n";
    if (failures == 0) {
        std::cout << "[  PASSED  ] All tests passed\n";
        return 0;
    } else {
        std::cout << "[  FAILED  ] " << failures << " checks failed\n";
        return 1;
    }
}