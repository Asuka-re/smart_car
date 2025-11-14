#include "executor.h"
#include <cctype>

using namespace adas;

Executor::Executor(Pose initial) noexcept : pose_(initial) {
    if (pose_.heading >= 'a' && pose_.heading <= 'z') {
        pose_.heading = static_cast<char>(pose_.heading - 'a' + 'A');
    }
}

void Executor::moveOne() noexcept {
    switch (pose_.heading) {
        case 'E': ++pose_.x; break;
        case 'W': --pose_.x; break;
        case 'N': ++pose_.y; break;
        case 'S': --pose_.y; break;
        default: break;
    }
}

void Executor::moveBack() noexcept {   // 倒车一格
    switch (pose_.heading) {
        case 'E': --pose_.x; break;
        case 'W': ++pose_.x; break;
        case 'N': --pose_.y; break;
        case 'S': ++pose_.y; break;
        default: break;
    }
}

void Executor::turnL() noexcept {
    pose_.heading = (pose_.heading == 'N') ? 'W' :
                    (pose_.heading == 'W') ? 'S' :
                    (pose_.heading == 'S') ? 'E' : 'N';
}

void Executor::turnR() noexcept {
    pose_.heading = (pose_.heading == 'N') ? 'E' :
                    (pose_.heading == 'E') ? 'S' :
                    (pose_.heading == 'S') ? 'W' : 'N';
}

void Executor::Execute(const std::string& commands) noexcept {
    for (char ch : commands) {
        char c = ch;
        if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');
        switch (c) {
            case 'F':
                accel_ = !accel_;
                break;
            case 'B':               // 倒车开关
                reverse_ = !reverse_;
                break;
            case 'M':
                reverse_ ? moveBack() : moveOne();
                if (accel_) reverse_ ? moveBack() : moveOne();
                break;
            case 'L':
                if (accel_) reverse_ ? moveBack() : moveOne();
                turnL();
                break;
            case 'R':
                if (accel_) reverse_ ? moveBack() : moveOne();
                turnR();
                break;
            default:
                break;
        }
    }
}

Pose Executor::Query() const noexcept {
    return pose_;
}