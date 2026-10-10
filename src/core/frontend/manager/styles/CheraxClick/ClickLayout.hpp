#pragma once
#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>

namespace YimMenu::ClickUI
{
    inline bool Matches(std::string_view text, std::string_view filter)
    {
        if (filter.empty()) return true;
        std::string a(text), b(filter);
        const auto lower = [](unsigned char c) { return static_cast<char>(std::tolower(c)); };
        std::transform(a.begin(), a.end(), a.begin(), lower);
        std::transform(b.begin(), b.end(), b.begin(), lower);
        return a.find(b) != std::string::npos;
    }
    constexpr int ColumnCount(float availableWidth) { return availableWidth >= 500 ? 2 : 1; }
    constexpr bool ReservePhone(bool open, bool list, bool click, bool onboarding)
    { return open && (list || click) && !onboarding; }
}
