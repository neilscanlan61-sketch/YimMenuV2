#include <algorithm>
#include <atomic>
#include <string>
#include "ModernList.hpp"
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
        const float height = std::max(44.0f, ImGui::GetTextLineHeight() + 14.0f);
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
            draw->AddRectFilled(start, ImVec2(start.x + width, start.y + height), IM_COL32(22, 63, 70, 255), 2.0f);
            draw->AddRectFilled(start, ImVec2(start.x + 3, start.y + height), IM_COL32(65, 221, 227, 255));
            s_FocusedLabel = label;
        }
        const auto valueSize = ImGui::CalcTextSize(value);
        const float baseline = start.y + (height - ImGui::GetTextLineHeight()) * 0.5f;
        draw->PushClipRect(ImVec2(start.x + 12, start.y), ImVec2(start.x + width - valueSize.x - 28, start.y + height), true);
        draw->AddText(ImVec2(start.x + 12, baseline), ImGui::GetColorU32(ImGuiCol_Text), label);
        draw->PopClipRect();
        draw->AddText(ImVec2(start.x + width - valueSize.x - 12, baseline),
            enabled ? IM_COL32(117, 238, 235, 255) : IM_COL32(175, 190, 197, 255), value);
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
        // Original vector header: no Impulse texture/assets or game/authentication code.
        draw->AddRectFilledMultiColor(p, ImVec2(p.x + width, p.y + height),
            IM_COL32(14, 41, 49, 255), IM_COL32(6, 15, 21, 255),
            IM_COL32(6, 15, 21, 255), IM_COL32(9, 28, 35, 255));
        draw->AddRectFilled(p, ImVec2(p.x + width, p.y + 3), IM_COL32(65, 221, 227, 255));
        const float fontSize = ImGui::GetFontSize();
        const float titleSize = fontSize * 1.65f;
        const char* title = "YIMMENU";
        const float titleWidth = ImGui::CalcTextSize(title).x * titleSize / fontSize;
        draw->AddText(ImGui::GetFont(), titleSize,
            ImVec2(p.x + (width - titleWidth) * 0.5f, p.y + 20), IM_COL32(239, 249, 249, 255), title);
        const char* subtitle = "V2  /  ENHANCED  /  KEYBOARD";
        const float subtitleSize = fontSize * 0.70f;
        const float subtitleWidth = ImGui::CalcTextSize(subtitle).x * subtitleSize / fontSize;
        draw->AddText(ImGui::GetFont(), subtitleSize,
            ImVec2(p.x + (width - subtitleWidth) * 0.5f, p.y + height - 27), IM_COL32(135, 207, 211, 255), subtitle);
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
                s.WindowPadding = ImVec2(12, 12);
                s.ItemSpacing = ImVec2(6, 2);
                s.FramePadding = ImVec2(10, 10);
                s.WindowRounding = 6;
                s.ChildRounding = 2;
                s.FrameRounding = 3;
                s.WindowBorderSize = 1;
                s.ScrollbarSize = 5;
                s.Colors[ImGuiCol_WindowBg] = ImVec4(0.035f, 0.047f, 0.06f, 0.98f);
                s.Colors[ImGuiCol_ChildBg] = ImVec4(0.045f, 0.06f, 0.075f, 1);
                s.Colors[ImGuiCol_PopupBg] = ImVec4(0.045f, 0.06f, 0.075f, 1);
                s.Colors[ImGuiCol_Text] = ImVec4(0.94f, 0.97f, 0.98f, 1);
                s.Colors[ImGuiCol_TextDisabled] = ImVec4(0.64f, 0.73f, 0.77f, 1);
                s.Colors[ImGuiCol_Border] = ImVec4(0.15f, 0.23f, 0.26f, 1);
                s.Colors[ImGuiCol_Header] = ImVec4(0.06f, 0.12f, 0.14f, 1);
                s.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.085f, 0.24f, 0.27f, 1);
                s.Colors[ImGuiCol_HeaderActive] = s.Colors[ImGuiCol_HeaderHovered];
                s.Colors[ImGuiCol_Button] = ImVec4(0.09f, 0.15f, 0.18f, 1);
                s.Colors[ImGuiCol_ButtonHovered] = s.Colors[ImGuiCol_HeaderHovered];
                s.Colors[ImGuiCol_ButtonActive] = s.Colors[ImGuiCol_HeaderActive];
                s.Colors[ImGuiCol_FrameBg] = ImVec4(0.07f, 0.11f, 0.14f, 1);
                s.Colors[ImGuiCol_FrameBgHovered] = s.Colors[ImGuiCol_HeaderHovered];
                s.Colors[ImGuiCol_FrameBgActive] = s.Colors[ImGuiCol_HeaderActive];
                s.Colors[ImGuiCol_CheckMark] = ImVec4(0.25f, 0.87f, 0.89f, 1);
                s.Colors[ImGuiCol_SliderGrab] = s.Colors[ImGuiCol_CheckMark];
                s.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.46f, 0.93f, 0.92f, 1);
                s.Colors[ImGuiCol_NavHighlight] = s.Colors[ImGuiCol_CheckMark];
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
        const ImVec2 size(std::max(280.0f, std::min(440.0f, io.DisplaySize.x - 32)),
            std::max(360.0f, std::min(700.0f, io.DisplaySize.y - 32)));
        ImGui::SetNextWindowSize(size, ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(32, 32), ImGuiCond_Always);
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
