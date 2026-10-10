#include <algorithm>
#include <atomic>
#include <string>
#include "ModernList.hpp"
#include "ListTheme.hpp"
#include "core/frontend/manager/UIManager.hpp"

namespace YimMenu::ListUI
{
    static bool s_Active = false;
    static std::atomic<bool> s_FocusRequested{true};
    static std::string s_FocusedLabel;
    static std::string s_Description;
    static std::string s_PendingValueLabel;
    bool Active() { return s_Active; }
    void RequestFocus() { s_FocusRequested.store(true); }

    void Describe(const char* description)
    {
        if (!s_Active || !(ImGui::IsItemFocused() || ImGui::IsItemActive())) return;
        if (!s_PendingValueLabel.empty()) s_FocusedLabel = s_PendingValueLabel;
        s_Description = description ? description : "";
    }

    bool Row(const void* id, const char* label, const char* value, bool enabled)
    {
        ImGui::PushID(id);
        const auto start = ImGui::GetCursorScreenPos();
        const float width = ImGui::GetContentRegionAvail().x;
        const float height = std::max(CurrentTheme().RowHeight, ImGui::GetTextLineHeight() + 14.0f);
        // Draw one filled keyboard-selection bar, not separate focus and mouse bars.
        ImGui::PushStyleColor(ImGuiCol_NavHighlight, ImVec4(0, 0, 0, 0));
        const bool activated = ImGui::Selectable("##list-row", false, 0, ImVec2(width, height));
        ImGui::PopStyleColor();
        const bool focused = ImGui::IsItemFocused();
        bool pressed = activated;
        if (focused && value[0] == '>' && value[1] == '\0'
            && ImGui::IsKeyPressed(ImGuiKey_RightArrow, false)) pressed = true;
        s_PendingValueLabel.clear();
        auto* draw = ImGui::GetWindowDrawList();
        if (focused)
        {
            draw->AddRectFilled(start, ImVec2(start.x + width, start.y + height), ImGui::GetColorU32(CurrentTheme().Selection), 2.0f);
            draw->AddRectFilled(start, ImVec2(start.x + 3, start.y + height), ImGui::GetColorU32(CurrentTheme().Accent));
            s_FocusedLabel = label;
        }
        const auto valueSize = ImGui::CalcTextSize(value);
        const float baseline = start.y + (height - ImGui::GetTextLineHeight()) * 0.5f;
        draw->PushClipRect(ImVec2(start.x + 12, start.y), ImVec2(start.x + width - valueSize.x - 28, start.y + height), true);
        draw->AddText(ImVec2(start.x + 12, baseline), ImGui::GetColorU32(ImGuiCol_Text), label);
        draw->PopClipRect();
        draw->AddText(ImVec2(start.x + width - valueSize.x - 12, baseline),
            ImGui::GetColorU32(enabled ? CurrentTheme().Accent : CurrentTheme().Muted), value);
        ImGui::PopID();
        return pressed;
    }

    void BeginValue(const char* label, float width)
    {
        s_PendingValueLabel = label;
        const auto start = ImGui::GetCursorScreenPos();
        const float available = ImGui::GetContentRegionAvail().x;
        width = std::min(width, available * 0.48f);
        auto* draw = ImGui::GetWindowDrawList();
        draw->PushClipRect(start, ImVec2(start.x + available - width - 16,
            start.y + ImGui::GetFrameHeight()), true);
        draw->AddText(ImVec2(start.x + 12, start.y + ImGui::GetStyle().FramePadding.y),
            ImGui::GetColorU32(ImGuiCol_Text), label);
        draw->PopClipRect();
        ImGui::SetCursorScreenPos(ImVec2(start.x + available - width, start.y));
        ImGui::SetNextItemWidth(width);
    }

    static void DrawBanner()
    {
        const auto p = ImGui::GetCursorScreenPos();
        const float width = ImGui::GetContentRegionAvail().x;
        const float height = std::max(94.0f, ImGui::GetTextLineHeight() * 4.0f);
        auto* draw = ImGui::GetWindowDrawList();
        // Original Cherax-inspired presentation, no proprietary branding/assets.
        draw->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(CurrentTheme().Header), CurrentTheme().Rounding);
        draw->AddRectFilled(ImVec2(p.x, p.y + height - 3), ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(CurrentTheme().Accent));
        const float fontSize = ImGui::GetFontSize();
        const float titleSize = fontSize * 1.65f;
        draw->AddText(ImGui::GetFont(), titleSize, ImVec2(p.x + 18, p.y + 17),
            ImGui::GetColorU32(CurrentTheme().Text), "YIMMENU");
        const char* subtitle = "ENHANCED  /  LIST INTERFACE";
        draw->AddText(ImGui::GetFont(), fontSize * 0.75f, ImVec2(p.x + 18, p.y + height - 29),
            ImGui::GetColorU32(CurrentTheme().Muted), subtitle);
        ImGui::Dummy(ImVec2(width, height));
    }
}

namespace YimMenu
{
    void RenderModernListTheme()
    {
        struct StyleScope
        {
            ImGuiStyle original;
            StyleScope() : original(ImGui::GetStyle())
            {
                ListUI::s_Active = true;
                ListUI::s_FocusedLabel.clear();
                ListUI::s_Description.clear();
                ListUI::s_PendingValueLabel.clear();
                auto& s = ImGui::GetStyle();
                const auto& theme = ListUI::CurrentTheme();
                s.WindowPadding = ImVec2(12, 12);
                s.ItemSpacing = ImVec2(8, 4);
                s.FramePadding = ImVec2(10, 10);
                s.WindowRounding = theme.Rounding;
                s.ChildRounding = 4;
                s.FrameRounding = 4;
                s.WindowBorderSize = 1;
                s.ScrollbarSize = 6;
                s.Colors[ImGuiCol_WindowBg] = theme.Background;
                s.Colors[ImGuiCol_ChildBg] = theme.Surface;
                s.Colors[ImGuiCol_PopupBg] = theme.Surface;
                s.Colors[ImGuiCol_Text] = theme.Text;
                s.Colors[ImGuiCol_TextDisabled] = theme.Muted;
                s.Colors[ImGuiCol_Border] = theme.Border;
                s.Colors[ImGuiCol_Header] = theme.Surface;
                s.Colors[ImGuiCol_HeaderHovered] = theme.Selection;
                s.Colors[ImGuiCol_HeaderActive] = theme.Selection;
                s.Colors[ImGuiCol_Button] = theme.Surface;
                s.Colors[ImGuiCol_ButtonHovered] = theme.Selection;
                s.Colors[ImGuiCol_ButtonActive] = theme.Selection;
                s.Colors[ImGuiCol_FrameBg] = theme.Background;
                s.Colors[ImGuiCol_FrameBgHovered] = theme.Selection;
                s.Colors[ImGuiCol_FrameBgActive] = theme.Selection;
                s.Colors[ImGuiCol_CheckMark] = theme.Accent;
                s.Colors[ImGuiCol_SliderGrab] = theme.Accent;
                s.Colors[ImGuiCol_SliderGrabActive] = theme.Text;
                s.Colors[ImGuiCol_NavHighlight] = theme.Accent;
            }
            ~StyleScope() { ImGui::GetStyle() = original; ListUI::s_Active = false; }
        } style;

        enum class Page { Home, Categories, Options };
        static Page page = Page::Home;
        static std::weak_ptr<Submenu> routeSubmenu;
        static std::weak_ptr<Category> routeCategory;
        auto submenu = routeSubmenu.lock();
        auto category = routeCategory.lock();
        const auto& submenus = UIManager::GetSubmenus();
        if (page != Page::Home && (!submenu || std::find(submenus.begin(), submenus.end(), submenu) == submenus.end()))
        {
            page = Page::Home;
            ListUI::RequestFocus();
        }
        if (page == Page::Options && (!category || std::find(submenu->m_Categories.begin(), submenu->m_Categories.end(), category) == submenu->m_Categories.end()))
        {
            page = Page::Categories;
            ListUI::RequestFocus();
        }

        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavNoCaptureKeyboard;
        // GUI::ToggleMouse owns NoMouse/NoMouseCursorChange, including theme transitions.
        const auto& theme = ListUI::CurrentTheme();
        const ImVec2 size(std::max(1.0f, std::min(theme.Width, io.DisplaySize.x - 32)),
            std::max(1.0f, std::min(theme.Height, io.DisplaySize.y - 32)));
        ImGui::SetNextWindowSize(size, ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(16, 16), ImGuiCond_Always);
        if (ListUI::s_FocusRequested.load()) ImGui::SetNextWindowFocus();
        UIManager::SetShowContentWindow(true);
        const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        if (ImGui::Begin("##YimModernListKeyboardV2", nullptr, flags))
        {
            ListUI::DrawBanner();
            ImGui::Spacing();
            const auto goBack = [&] {
                page = page == Page::Options ? Page::Categories : Page::Home;
                ListUI::RequestFocus();
            };
            if (!io.WantTextInput && !ImGui::IsAnyItemActive()
                && !ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel)
                && ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
            {
                if (ImGui::IsKeyPressed(ImGuiKey_Backspace, false)
                    || (page == Page::Categories && ImGui::IsKeyPressed(ImGuiKey_LeftArrow, false))) goBack();
            }
            std::string breadcrumb = "Home";
            if (page != Page::Home && submenu) breadcrumb = "Home / " + submenu->m_Name;
            if (page == Page::Options && category) breadcrumb += " / " + category->m_Name;
            ImGui::TextUnformatted(breadcrumb.c_str());
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::PushID(page == Page::Home ? nullptr : submenu.get());
            ImGui::PushID(page == Page::Options ? category.get() : nullptr);
            const char* childId = page == Page::Home ? "##home-list" : page == Page::Categories ? "##category-list" : "##option-list";
            if (ImGui::BeginChild(childId, ImVec2(0, -92), false, ImGuiWindowFlags_HorizontalScrollbar))
            {
                // Focus the first real content item, not a mouse-only Back button.
                if (ListUI::s_FocusRequested.exchange(false)) ImGui::SetKeyboardFocusHere(0);
                if (page == Page::Home)
                {
                    for (const auto& entry : submenus)
                    {
                        if (!entry) continue;
                        if (ListUI::Row(entry.get(), entry->m_Name.c_str()))
                        {
                            UIManager::SetActiveSubmenu(entry);
                            routeSubmenu = entry;
                            page = Page::Categories;
                            ListUI::RequestFocus();
                            break;
                        }
                    }
                }
                else if (page == Page::Categories && submenu)
                {
                    for (const auto& entry : submenu->m_Categories)
                    {
                        if (!entry) continue;
                        if (ListUI::Row(entry.get(), entry->m_Name.c_str()))
                        {
                            submenu->SetActiveCategory(entry);
                            routeCategory = entry;
                            page = Page::Options;
                            ListUI::RequestFocus();
                            break;
                        }
                    }
                    if (submenu->m_Categories.empty()) ImGui::TextDisabled("No categories available.");
                }
                else if (category)
                {
                    auto* font = UIManager::GetOptionsFont();
                    if (font) ImGui::PushFont(font);
                    category->Draw();
                    if (font) ImGui::PopFont();
                }
            }
            ImGui::EndChild();
            ImGui::PopID();
            ImGui::PopID();
            ImGui::Separator();
            // Bound the description region so long text cannot overlap the controls.
            if (ImGui::BeginChild("##description", ImVec2(0, 48), false))
            {
                const auto text = !ListUI::s_Description.empty() ? ListUI::s_Description
                    : !ListUI::s_FocusedLabel.empty() ? ListUI::s_FocusedLabel
                    : "Up/Down: select a row. Enter: activate.";
                ImGui::TextWrapped("%s", text.c_str());
            }
            ImGui::EndChild();
            ImGui::TextDisabled("Up/Down: select  Enter: open  Backspace: back");
        }
        ImGui::End();
    }
}
