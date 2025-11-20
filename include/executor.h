#pragma once
#include <string>

namespace adas {
struct Pose {
    int x{};
    int y{};
    char heading{'N'};
};

class Executor {
public:
    static Executor* NewExecutor(const Pose& pose = {0, 0, 'N'}) noexcept;
public:
    Executor() = default;
    virtual ~Executor() = default;
    Executor(const Executor&)            = delete;
    Executor& operator=(const Executor&) = delete;
public:
    virtual void Execute(const std::string& commands) noexcept = 0;
    virtual Pose Query() const noexcept                        = 0;
};

inline bool operator==(const Pose& lhs, const Pose& rhs) noexcept {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.heading == rhs.heading;
}
}  // namespace adas