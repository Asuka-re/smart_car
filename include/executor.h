#pragma once
#include <string>
#include <cstdint>

namespace adas {

struct Pose {
    int32_t x{0};
    int32_t y{0};
    char heading{'N'};
};

class Executor {
public:
    explicit Executor(Pose initial = Pose{}) noexcept;
    void Execute(const std::string& commands) noexcept;
    Pose Query() const noexcept;
private:
    Pose pose_;
    bool accel_{false};
    void moveOne() noexcept;
    void turnL() noexcept;
    void turnR() noexcept;
};

} // namespace adas