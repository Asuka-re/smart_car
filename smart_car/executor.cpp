#include "executor.h"
#include <new>

namespace adas {

class ExecutorImpl final : public Executor {
public:
    explicit ExecutorImpl(const Pose& pose) noexcept : pose_(pose) {}
    void Execute(const std::string& commands) noexcept override;
    Pose Query() const noexcept override { return pose_; }
private:
    Pose pose_;
};

Executor* Executor::NewExecutor(const Pose& pose) noexcept {
    return new (std::nothrow) ExecutorImpl(pose);
}

void ExecutorImpl::Execute(const std::string& commands) noexcept {
    for (char cmd : commands) {
        switch (cmd) {
        case 'M':
            if (pose_.heading == 'E') ++pose_.x;
            else if (pose_.heading == 'W') --pose_.x;
            else if (pose_.heading == 'N') ++pose_.y;
            else if (pose_.heading == 'S') --pose_.y;
            break;
        case 'L':
            pose_.heading = (pose_.heading == 'N') ? 'W' :
                            (pose_.heading == 'W') ? 'S' :
                            (pose_.heading == 'S') ? 'E' : 'N';
            break;
        case 'R':
            pose_.heading = (pose_.heading == 'N') ? 'E' :
                            (pose_.heading == 'E') ? 'S' :
                            (pose_.heading == 'S') ? 'W' : 'N';
            break;
        }
    }
}

}  // namespace adas