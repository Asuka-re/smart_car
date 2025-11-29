// CmderFactory 解析规则：
//  - 支持替换 "TR" 为 'Z' 简化后续映射。
//  - 空格仅分隔视觉分组，不会触发 Fast/Reverse 自动复位（状态持久 A/D）。
//  - F/B 字符即时切换对应状态（翻转），其后命令受影响直到再次出现同字符。
//  - 未知字符忽略（保持鲁棒性）。
//  - 输出为顺序 Cmder（可调用 operator()(PoseHandler&) -> ActionGroup）。
#include "CmderFactory.hpp"
#include <string>

namespace adas {
CmderList CmderFactory::GetCmders(const std::string& commands) const noexcept {
    CmderList cmders;
    const auto parsed = ParseCommandString(commands);
    // 语义更新：Fast(B)/Reverse(F) 状态跨空格分组持久，直到再次出现对应字符才切换（不自动复位）
    // 因此这里只解析顺序，不执行组尾复位逻辑。
    size_t start = 0;
    while (start <= parsed.size()) {
        const auto end = parsed.find(' ', start);
        const auto token = parsed.substr(start, (end == std::string::npos ? parsed.size() : end) - start);
        if (!token.empty()) {
            for (const char ch : token) {
                if (ch == 'F') { cmders.push_back(FastCommand()); continue; }
                if (ch == 'B') { cmders.push_back(ReverseCommand()); continue; }
                if (const auto it = cmderMap_.find(ch); it != cmderMap_.end()) {
                    cmders.push_back(it->second);
                }
            }
        }
        if (end == std::string::npos) break;
        start = end + 1;
    }
    return cmders;
}

std::string CmderFactory::ParseCommandString(std::string_view commands) const noexcept {
    std::string result(commands);
    ReplaceAll(result, "TR", "Z");
    return result;
}

void CmderFactory::ReplaceAll(std::string& inout, std::string_view what, 
                              std::string_view with) const noexcept {
    for (size_t pos = 0; 
         inout.npos != (pos = inout.find(what.data(), pos, what.length()));
         pos += with.length()) {
        inout.replace(pos, what.length(), with.data(), with.length());
    }
}
}  // namespace adas