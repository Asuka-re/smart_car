#pragma once
#include <string>
#include <utility>

namespace adas {
struct Pose {
    int32_t x{};
    int32_t y{};
    char    heading{'N'};
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
}  // namespace adas