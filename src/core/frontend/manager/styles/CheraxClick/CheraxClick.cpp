#include "CheraxClick.hpp"
#include "core/renderer/Renderer.hpp"
#include "ClickTheme.hpp"
#include "ClickLayout.hpp"
#include "ClickBookmarks.hpp"
#include "core/frontend/manager/UIManager.hpp"
#include "game/frontend/Menu.hpp"
#include "game/frontend/items/Items.hpp"
#include <algorithm>
#include <cmath>

namespace YimMenu::ClickUI
{
    static bool s_Active = false;
    static char s_Search[96]{};
    static Bookmarks s_Bookmarks;
    bool Active() { return s_Active; }

    void DrawPanel(const void* id, const std::string& name, const std::function<void()>& contents)
    {
        ImGui::PushID(id);
        auto* draw = ImGui::GetWindowDrawList();
        ImDrawListSplitter splitter;
        splitter.Split(draw, 2); splitter.SetCurrentChannel(draw, 1);
        const auto start = ImGui::GetCursorScreenPos();
        const float width = ImGui::GetContentRegionAvail().x;
        ImGui::BeginGroup();
        ImGui::Dummy(ImVec2(width, 6));
        ImGui::Indent(8);
        if (!name.empty())
        {
            const auto textSize = ImGui::CalcTextSize(name.c_str());
            const float x = ImGui::GetCursorPosX();
            ImGui::SetCursorPosX(x + std::max(0.0f, (width - 16 - textSize.x) * 0.5f));
            ImGui::TextColored(CurrentTheme().Accent, "%s", name.c_str());
            ImGui::SetCursorPosX(x);
            ImGui::Separator();
        }
        // Every existing item remains responsible for its own command binding.
        ImGui::PushItemWidth(std::max(1.0f, width - 16));
        contents();
        ImGui::PopItemWidth();
        ImGui::Unindent(8);
        ImGui::Dummy(ImVec2(width, 8));
        ImGui::EndGroup();
        const auto end = ImGui::GetItemRectMax();
        splitter.SetCurrentChannel(draw, 0);
        const ImVec2 bounds(start.x + width, end.y);
        draw->AddRectFilled(start, bounds, ImGui::GetColorU32(CurrentTheme().Surface), CurrentTheme().Rounding);
        draw->PushClipRect(start, bounds, true);
        const ImVec4 stripe(1,1,1,CurrentTheme().Scanlines);
        for (float y = start.y + 3; y < bounds.y; y += 5)
            draw->AddLine(ImVec2(start.x,y), ImVec2(bounds.x,y), ImGui::GetColorU32(stripe));
        draw->PopClipRect();
        draw->AddRect(start, bounds, ImGui::GetColorU32(CurrentTheme().Border), CurrentTheme().Rounding);
        splitter.Merge(draw);
        ImGui::Spacing();
        ImGui::PopID();
    }

    void DrawCategory(Category& category)
    {
        std::vector<std::shared_ptr<UIItem>> cards;
        for (const auto& item : category.GetItems())
        {
            if (!item || !item->CanDraw()) continue;
            auto* group = dynamic_cast<Group*>(item.get());
            if (group)
            {
                if (Matches(group->GetName(), s_Search) || Matches(category.m_Name, s_Search)) cards.push_back(item);
            }
            else
            {
                // Complex original custom editors and tab bars retain full width.
                // They cannot safely be reduced to fake screenshot controls.
                ImGui::PushID(item.get()); item->Draw(); ImGui::PopID();
            }
        }
        if (cards.empty()) return;
        const int columns = ColumnCount(ImGui::GetContentRegionAvail().x);
        if (columns == 1)
        {
            for (const auto& item : cards) item->Draw();
            return;
        }
        const float width = std::max(1.0f, (ImGui::GetContentRegionAvail().x - 8) * 0.5f);
        const float height = std::max(1.0f, ImGui::GetContentRegionAvail().y);
        for (int column = 0; column < columns; ++column)
        {
            if (column) ImGui::SameLine(0, 8);
            ImGui::PushID(column);
            if (ImGui::BeginChild("##cards", ImVec2(width, height), false))
                for (size_t i = column; i < cards.size(); i += columns) cards[i]->Draw();
            ImGui::EndChild(); ImGui::PopID();
        }
    }

    struct StyleScope
    {
        ImGuiStyle Previous;
        bool PreviousActive;
        StyleScope() : Previous(ImGui::GetStyle()), PreviousActive(s_Active)
        {
            s_Active = true;
            auto& s = ImGui::GetStyle(); const auto& t = CurrentTheme();
            s.WindowPadding = ImVec2(8,8); s.FramePadding = ImVec2(5,3);
            s.ItemSpacing = ImVec2(5,4); s.ItemInnerSpacing = ImVec2(5,3);
            s.WindowRounding = t.Rounding; s.ChildRounding = t.Rounding;
            const float rounded = std::clamp(t.Rounding, 6.0f, 12.0f);
            s.FrameRounding = rounded; s.GrabRounding = rounded; s.TabRounding = rounded;
            s.FrameBorderSize = 1;
            s.ScrollbarSize = 7; s.ScrollbarRounding = 4;
            s.WindowBorderSize = 1; s.ChildBorderSize = 1;
            s.Colors[ImGuiCol_WindowBg] = t.Background;
            s.Colors[ImGuiCol_ChildBg] = ImVec4(0,0,0,0);
            s.Colors[ImGuiCol_PopupBg] = t.Background;
            s.Colors[ImGuiCol_PopupBg].w = 1.0f;
            s.Colors[ImGuiCol_Text] = t.Text; s.Colors[ImGuiCol_TextDisabled] = t.Muted;
            s.Colors[ImGuiCol_Border] = t.Border;
            s.Colors[ImGuiCol_Button] = t.Button; s.Colors[ImGuiCol_ButtonHovered] = t.ButtonHover;
            s.Colors[ImGuiCol_ButtonActive] = t.Selection;
            s.Colors[ImGuiCol_Header] = t.Selection;
            s.Colors[ImGuiCol_HeaderHovered] = t.ButtonHover; s.Colors[ImGuiCol_HeaderActive] = t.Accent;
            s.Colors[ImGuiCol_FrameBg] = t.Surface;
            s.Colors[ImGuiCol_FrameBgHovered] = t.Selection; s.Colors[ImGuiCol_FrameBgActive] = t.Selection;
            s.Colors[ImGuiCol_CheckMark] = t.Accent;
            s.Colors[ImGuiCol_SliderGrab] = t.Accent; s.Colors[ImGuiCol_SliderGrabActive] = t.Text;
            s.Colors[ImGuiCol_ScrollbarBg] = t.Surface;
            s.Colors[ImGuiCol_ScrollbarGrab] = t.Accent;
            s.Colors[ImGuiCol_ScrollbarGrabHovered] = t.Text;
            s.Colors[ImGuiCol_ScrollbarGrabActive] = t.Text;
            s.Colors[ImGuiCol_Tab] = t.Header; s.Colors[ImGuiCol_TabActive] = t.Selection;
            s.Colors[ImGuiCol_TabHovered] = t.ButtonHover; s.Colors[ImGuiCol_Separator] = t.Border;
        }
        ~StyleScope() { ImGui::GetStyle() = Previous; s_Active = PreviousActive; }
    };

    static bool IconButton(const void* id, const char* icon, const char* fallback, bool selected)
    {
        ImGui::PushID(id);
        ImGui::PushStyleColor(ImGuiCol_Button, selected ? CurrentTheme().Selection : ImVec4(0,0,0,0));
        auto* font = Menu::Font::g_AwesomeFont;
        if (font && icon && icon[0]) ImGui::PushFont(font);
        const bool clicked = ImGui::Button(font && icon && icon[0] ? icon : fallback, ImVec2(38,36));
        if (font && icon && icon[0]) ImGui::PopFont();
        ImGui::PopStyleColor(); ImGui::PopID();
        return clicked;
    }
}

namespace YimMenu
{
    void RenderCheraxClickTheme()
    {
        ClickUI::StyleScope style;
        auto& io = ImGui::GetIO(); const auto& theme = ClickUI::CurrentTheme();
        static bool bookmarksLoaded = false;
        if (!bookmarksLoaded && !ClickUI::ThemeFolder().empty())
        {
            ClickUI::s_Bookmarks.Load(ClickUI::ThemeFolder() / "bookmarks.txt"); bookmarksLoaded = true;
        }
        const ImVec2 size(std::max(1.0f,std::min(theme.Width,io.DisplaySize.x-32)),
            std::max(1.0f,std::min(theme.Height,io.DisplaySize.y-32)));
        ImGui::SetNextWindowSize(size, ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(32,32), ImGuiCond_FirstUseEver);
        UIManager::SetShowContentWindow(true);
        ImVec2 windowPos, windowSize;
        bool showPlayerInfo = false;
        const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;
        if (ImGui::Begin("##YimCheraxClick", nullptr, flags))
        {
            windowPos = ImGui::GetWindowPos(); windowSize = ImGui::GetWindowSize();
            auto* draw = ImGui::GetWindowDrawList();
            draw->AddRectFilled(windowPos, ImVec2(windowPos.x+windowSize.x,windowPos.y+windowSize.y),
                ImGui::GetColorU32(theme.Background), theme.Rounding);
            const float inset = std::clamp(theme.Rounding, 0.0f, 16.0f);
            draw->AddRectFilledMultiColor(ImVec2(windowPos.x+inset,windowPos.y+inset),
                ImVec2(windowPos.x+windowSize.x-inset,windowPos.y+windowSize.y-inset),
                ImGui::GetColorU32(theme.Background),ImGui::GetColorU32(theme.Background),
                ImGui::GetColorU32(theme.BackgroundLow),ImGui::GetColorU32(theme.BackgroundLow));
            if (theme.BackgroundEnabled >= 0.5f)
                Renderer::DrawAnimatedBackground(draw,windowPos,windowSize,theme.BackgroundOpacity,theme.Rounding);
            // Static soft light pools: no timers, flicker, shader changes or new assets.
            // Animated motion, if desired, comes from the existing GIF decoder.
            if (theme.LightIntensity > 0)
            {
                draw->PushClipRect(ImVec2(windowPos.x+inset,windowPos.y+inset),
                    ImVec2(windowPos.x+windowSize.x-inset,windowPos.y+windowSize.y-inset),true);
                for (int orb=0; orb<3; ++orb)
                {
                    const ImVec2 center(windowPos.x+windowSize.x*(orb==0?0.18f:orb==1?0.82f:0.5f),
                        windowPos.y+windowSize.y*(orb==0?0.8f:orb==1?0.26f:0.9f));
                    const float radius=std::min(windowSize.x,windowSize.y)*0.27f;
                    for (int ring=12; ring>0; --ring)
                    {
                        auto color=orb==1?theme.Accent:theme.BackgroundLow;
                        color.w=std::clamp(theme.LightIntensity,0.0f,0.6f)*(13-ring)/156.0f;
                        draw->AddCircleFilled(center,radius*ring/12.0f,ImGui::GetColorU32(color),32);
                    }
                }
                draw->PopClipRect();
            }
            const auto header = ImGui::GetCursorScreenPos(); const float available = ImGui::GetContentRegionAvail().x;
            // User-selected branding; retain YimMenu attribution in the project docs.
            constexpr const char* brand = "ROCKSTAR TOOL";
            const float unscaledLogoWidth = ImGui::CalcTextSize(brand).x;
            const float logoArea = std::max(1.0f, available - 236);
            const float logoScale = std::min(1.2f, std::max(0.5f, (logoArea - 16) / std::max(1.0f,unscaledLogoWidth)));
            const float logoSize = ImGui::GetFontSize()*logoScale;
            const float logoWidth = unscaledLogoWidth*logoScale;
            ImGui::InvisibleButton("##header-drag",ImVec2(std::max(1.0f,available-220),36));
            if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
            {
                const auto delta = io.MouseDelta;
                ImGui::SetWindowPos(ImVec2(std::clamp(windowPos.x+delta.x,0.0f,std::max(0.0f,io.DisplaySize.x-windowSize.x)),
                    std::clamp(windowPos.y+delta.y,0.0f,std::max(0.0f,io.DisplaySize.y-windowSize.y))));
            }
            draw->AddText(ImGui::GetFont(),logoSize,ImVec2(header.x+std::max(8.0f,(logoArea-logoWidth)*0.5f),header.y+2),
                ImGui::GetColorU32(theme.Text),brand);
            ImGui::SetCursorScreenPos(ImVec2(header.x+std::max(0.0f,available-214),header.y));
            ImGui::SetNextItemWidth(130);
            ImGui::InputTextWithHint("##click-search","Search sections",ClickUI::s_Search,sizeof(ClickUI::s_Search));
            ImGui::SameLine();
            const bool themeClicked=ImGui::Button("##theme-icon",ImVec2(28,28));
            if (Menu::Font::g_AwesomeFont)
                draw->AddText(Menu::Font::g_AwesomeFont,18,ImVec2(ImGui::GetItemRectMin().x+5,ImGui::GetItemRectMin().y+5),ImGui::GetColorU32(theme.Text),"\xef\x87\xbc");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Custom themes");
            if (themeClicked) ImGui::OpenPopup("Click UI themes");
            ImGui::SameLine();
            const bool starClicked=ImGui::Button("##bookmark-icon",ImVec2(28,28));
            auto selectedSubmenu=UIManager::GetActiveSubmenu();
            auto selectedCategory=selectedSubmenu?selectedSubmenu->GetActiveCategory():nullptr;
            const bool saved=selectedCategory && ClickUI::s_Bookmarks.Contains(selectedSubmenu->m_Name,selectedCategory->m_Name);
            if (Menu::Font::g_AwesomeFont)
                draw->AddText(Menu::Font::g_AwesomeFont,18,ImVec2(ImGui::GetItemRectMin().x+5,ImGui::GetItemRectMin().y+5),ImGui::GetColorU32(saved?theme.Accent:theme.Text),"\xef\x80\x85");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Save / unsave this section");
            if (starClicked && selectedCategory) ClickUI::s_Bookmarks.Toggle(selectedSubmenu->m_Name,selectedCategory->m_Name);
            ImGui::SetCursorScreenPos(ImVec2(header.x,header.y+40));
            ImGui::Separator();
            const auto& submenus = UIManager::GetSubmenus(); auto active = UIManager::GetActiveSubmenu();
            if (!active || std::find(submenus.begin(),submenus.end(),active)==submenus.end())
            {
                active = submenus.empty()?nullptr:submenus.front(); if (active) UIManager::SetActiveSubmenu(active);
            }
            if (ImGui::BeginChild("##icon-rail",ImVec2(44,0),false))
            {
                // Order approximates the screenshot while preserving dynamic/Lua submenus.
                std::vector<std::shared_ptr<Submenu>> ordered;
                for (const char* name : {"Self","Players","Network","Teleport","Vehicle","World","Recovery","Settings","Debug"})
                    for (const auto& entry : submenus) if (entry && entry->m_Name==name) ordered.push_back(entry);
                for (const auto& entry : submenus)
                    if (entry && std::find(ordered.begin(),ordered.end(),entry)==ordered.end()) ordered.push_back(entry);
                for (const auto& entry : ordered)
                {
                    if (ClickUI::IconButton(entry.get(),entry->m_Icon.c_str(),entry->m_Name.substr(0,1).c_str(),entry==active))
                    {
                        UIManager::SetActiveSubmenu(entry); active=entry; ClickUI::s_Search[0]='\0';
                    }
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s",entry->m_Name.c_str());
                }
            }
            ImGui::EndChild(); ImGui::SameLine(0,8);
            if (ImGui::BeginChild("##click-body",ImVec2(0,0),false) && active)
            {
                auto category = active->GetActiveCategory();
                if (!category || std::find(active->m_Categories.begin(),active->m_Categories.end(),category)==active->m_Categories.end())
                {
                    category=active->m_Categories.empty()?nullptr:active->m_Categories.front();
                    if (category) active->SetActiveCategory(category);
                }
                if (active->m_Name=="Players" && ImGui::GetContentRegionAvail().x>=540)
                {
                    if (ImGui::BeginChild("##players-dock",ImVec2(theme.PlayerWidth,0),true)) Submenus::DrawClickPlayerDock();
                    ImGui::EndChild(); ImGui::SameLine(0,8); showPlayerInfo=true;
                }
                if (ImGui::BeginChild("##category-body",ImVec2(0,0),false))
                {
                    // Category selector is a scrollable horizontal strip, not a list.
                    if (ImGui::BeginChild("##click-tabs",ImVec2(0,36),false,ImGuiWindowFlags_HorizontalScrollbar))
                    {
                        bool first=true;
                        for (const auto& entry : active->m_Categories)
                        {
                            if (!entry) continue;
                            if (!first) ImGui::SameLine(0,4);
                            first=false;
                            ImGui::PushID(entry.get());
                            ImGui::PushStyleColor(ImGuiCol_Button,entry==category?theme.Selection:theme.Header);
                            if (ImGui::Button(entry->m_Name.c_str(),ImVec2(0,28))) { active->SetActiveCategory(entry); category=entry; }
                            ImGui::PopStyleColor(); ImGui::PopID();
                        }
                    }
                    ImGui::EndChild();
                    if (category)
                    {
                        ImGui::PushID(category.get());
                        const bool starred=ClickUI::s_Bookmarks.Contains(active->m_Name,category->m_Name);
                        if (ImGui::SmallButton(starred?"* Saved":"+ Save")) ClickUI::s_Bookmarks.Toggle(active->m_Name,category->m_Name);
                        ImGui::SameLine();
                        if (ImGui::SmallButton("Favorites")) ImGui::OpenPopup("Saved sections");
                        if (ImGui::BeginPopup("Saved sections"))
                        {
                            if (ClickUI::s_Bookmarks.All().empty()) ImGui::TextDisabled("No saved sections yet.");
                            for (const auto& saved : ClickUI::s_Bookmarks.All())
                                if (ImGui::Selectable((saved.Submenu+" / "+saved.Category).c_str()))
                                    for (const auto& sub : submenus) if (sub && sub->m_Name==saved.Submenu)
                                        for (const auto& cat : sub->m_Categories) if (cat && cat->m_Name==saved.Category)
                                        { UIManager::SetActiveSubmenu(sub); sub->SetActiveCategory(cat); }
                            ImGui::EndPopup();
                        }
                        if (!ClickUI::s_Bookmarks.Error().empty()) ImGui::TextWrapped("%s",ClickUI::s_Bookmarks.Error().c_str());
                        if (ImGui::BeginChild("##click-options",ImVec2(0,0),false))
                        {
                            auto* font=UIManager::GetOptionsFont(); if (font) ImGui::PushFont(font);
                            category->Draw(); if (font) ImGui::PopFont();
                        }
                        ImGui::EndChild(); ImGui::PopID();
                    }
                    else ImGui::TextDisabled("No categories in this section.");
                }
                ImGui::EndChild();
            }
            ImGui::EndChild();
            ImGui::SetNextWindowSize(ImVec2(std::min(440.0f,io.DisplaySize.x-32),std::min(390.0f,io.DisplaySize.y-32)),ImGuiCond_Appearing);
            if (ImGui::BeginPopupModal("Click UI themes",nullptr,ImGuiWindowFlags_NoCollapse))
            {
                if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();
                if (ImGui::BeginChild("##theme-editor",ImVec2(0,0),false)) ClickUI::DrawThemeEditor();
                ImGui::EndChild(); ImGui::EndPopup();
            }
        }
        ImGui::End();
        // Optional read-only player information dock; don't create new remote actions.
        if (showPlayerInfo && windowPos.x+windowSize.x+280<io.DisplaySize.x)
        {
            ImGui::SetNextWindowPos(ImVec2(windowPos.x+windowSize.x+8,windowPos.y),ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(260,300),ImGuiCond_Always);
            if (ImGui::Begin("##click-player-info",nullptr,ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize)) Submenus::DrawClickPlayerInfo();
            ImGui::End();
        }
    }
}
