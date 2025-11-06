#include "executor.h"
#include <iostream>
#include <memory>

namespace adas {
bool operator==(const Pose& a, const Pose& b) {
    return a.x == b.x && a.y == b.y && a.heading == b.heading;
}
bool test(const char* name, bool cond) {
    std::cout << (cond ? "[PASS] " : "[FAIL] ") << name << '\n';
    return cond;
}
int runAllTests() {
    bool all = true;

    /* ---------- 用例1&2：初始化场景 ---------- */
    all &= test("should_return_init_pose_when_without_command",
                []{
                    auto e = std::unique_ptr<Executor>(Executor::NewExecutor({5, 5, 'E'}));
                    return e->Query() == Pose({5, 5, 'E'});
                }());

    all &= test("should_return_default_pose_when_without_init_and_command",
                []{
                    auto e = std::unique_ptr<Executor>(Executor::NewExecutor());
                    return e->Query() == Pose({0, 0, 'N'});
                }());

    /* ---------- 用例3~6：移动指令正交 ---------- */
    all &= test("should_return_x_plus_1_given_command_is_M_and_facing_is_E", []{
        auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0, 0, 'E'}));
        e->Execute("M");
        return e->Query() == Pose({1, 0, 'E'});
    }());

    all &= test("should_return_x_minus_1_given_command_is_M_and_facing_is_W", []{
        auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0, 0, 'W'}));
        e->Execute("M");
        return e->Query() == Pose({-1, 0, 'W'});
    }());

    all &= test("should_return_y_plus_1_given_command_is_M_and_facing_is_N", []{
        auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0, 0, 'N'}));
        e->Execute("M");
        return e->Query() == Pose({0, 1, 'N'});
    }());

    all &= test("should_return_y_minus_1_given_command_is_M_and_facing_is_S", []{
        auto e = std::unique_ptr<Executor>(Executor::NewExecutor({0, 0, 'S'}));
        e->Execute("M");
        return e->Query() == Pose({0, -1, 'S'});
    }());

    std::cout << (all ? "\nAll tests PASSED.\n" : "\nSome tests FAILED.\n");
    return all ? 0 : 1;
}
}  // namespace adas

int main() {
    return adas::runAllTests();
}