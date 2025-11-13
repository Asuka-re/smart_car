#include "executor.h"
#include <iostream>
#include <string>

using namespace adas;

int main(int argc, char** argv) {
    if (argc != 2 && argc != 5) {
        std::cerr << "Usage:\n  " << argv[0] << " CMD\n  " << argv[0] << " x y H CMD\n";
        return 1;
    }
    Pose start{};
    std::string cmds;
    if (argc == 2) {
        cmds = argv[1];
    } else {
        try {
            start.x = std::stoi(argv[1]);
            start.y = std::stoi(argv[2]);
            start.heading = argv[3][0];
            cmds = argv[4];
        } catch (...) {
            std::cerr << "Invalid arguments\n";
            return 1;
        }
    }
    Executor exe(start);
    exe.Execute(cmds);
    auto p = exe.Query();
    std::cout << p.x << ' ' << p.y << ' ' << p.heading << '\n';
    return 0;
}