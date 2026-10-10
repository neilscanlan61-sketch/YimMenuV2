#pragma once
#include <functional>
#include <string>

namespace YimMenu { class Category; }
namespace YimMenu::ClickUI
{
    bool Active();
    void DrawCategory(Category& category);
    void DrawPanel(const void* id, const std::string& name, const std::function<void()>& contents);
}
namespace YimMenu::Submenus
{
    void DrawClickPlayerDock();
    void DrawClickPlayerInfo();
}
namespace YimMenu { void RenderCheraxClickTheme(); }
