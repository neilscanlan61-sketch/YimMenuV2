#include <algorithm>
#include <string>
#include "ModernList.hpp"
#include "core/frontend/manager/UIManager.hpp"

namespace YimMenu::ListUI
{
    static bool s_Active = false;
    bool Active() { return s_Active; }

    bool Row(const void* id, const char* label, const char* value, bool selected)
    {
        ImGui::PushID(id);
        const auto start = ImGui::GetCursorScreenPos();
        const float width = ImGui::GetContentRegionAvail().x;
        const float height = std::max(40.0f, ImGui::GetTextLineHeight() + 16.0f);
        const bool pressed = ImGui::Selectable("##list-row", selected, 0, ImVec2(width, height));
        auto* draw = ImGui::GetWindowDrawList();
        const bool highlight = selected || ImGui::IsItemHovered() || ImGui::IsItemFocused();
        if (highlight)
            draw->AddRectFilled(start, ImVec2(start.x + 3.0f, start.y + height), IM_COL32(94, 159, 232, 255), 1.0f);
        const auto valueSize = ImGui::CalcTextSize(value);
        const float baseline = start.y + (height - ImGui::GetTextLineHeight()) * 0.5f;
        draw->PushClipRect(ImVec2(start.x + 12.0f, start.y), ImVec2(start.x + width - valueSize.x - 28.0f, start.y + height), true);
        draw->AddText(ImVec2(start.x + 12.0f, baseline), ImGui::GetColorU32(ImGuiCol_Text), label);
        draw->PopClipRect();
        draw->AddText(ImVec2(start.x + width - valueSize.x - 12.0f, baseline),
            selected ? IM_COL32(144, 193, 255, 255) : IM_COL32(177, 185, 199, 255), value);
        ImGui::PopID();
        return pressed;
    }

    void BeginValue(const char* label, float width)
    {
        // Keep the command label at left and the editable value at right.
        const auto start = ImGui::GetCursorScreenPos();
        const float available = ImGui::GetContentRegionAvail().x;
        width = std::min(width, available * 0.48f);
        auto* draw = ImGui::GetWindowDrawList();
        draw->PushClipRect(start, ImVec2(start.x + available - width - 16.0f,
            start.y + ImGui::GetFrameHeight()), true);
        draw->AddText(ImVec2(start.x + 12.0f, start.y + ImGui::GetStyle().FramePadding.y),
            ImGui::GetColorU32(ImGuiCol_Text), label);
        draw->PopClipRect();
        ImGui::SetCursorScreenPos(ImVec2(start.x + available - width, start.y));
        ImGui::SetNextItemWidth(width);
    }
}

namespace YimMenu
{
    void RenderModernListTheme()
    {
        // Changes are scoped: do not overwrite saved styles, other layouts, or overlays.
        struct StyleScope
        {
            ImGuiStyle original;
            StyleScope() : original(ImGui::GetStyle())
            {
                ListUI::s_Active = true;
                auto& s = ImGui::GetStyle();
                s.WindowPadding = ImVec2(16, 16);
                s.ItemSpacing = ImVec2(8, 6);
                s.FramePadding = ImVec2(10, 10);
                s.WindowRounding = 10;
                s.ChildRounding = 6;
                s.FrameRounding = 5;
                s.WindowBorderSize = 1;
                s.ScrollbarSize = 6;
                s.Colors[ImGuiCol_WindowBg] = ImVec4(0.065f, 0.075f, 0.095f, 0.98f);
                s.Colors[ImGuiCol_ChildBg] = ImVec4(0.085f, 0.095f, 0.12f, 1);
                s.Colors[ImGuiCol_PopupBg] = ImVec4(0.085f, 0.095f, 0.12f, 1);
                s.Colors[ImGuiCol_Text] = ImVec4(0.94f, 0.95f, 0.97f, 1);
                s.Colors[ImGuiCol_TextDisabled] = ImVec4(0.69f, 0.73f, 0.79f, 1);
                s.Colors[ImGuiCol_Border] = ImVec4(0.23f, 0.27f, 0.34f, 1);
                s.Colors[ImGuiCol_Header] = ImVec4(0.16f, 0.24f, 0.35f, 1);
                s.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.18f, 0.27f, 0.40f, 1);
                s.Colors[ImGuiCol_HeaderActive] = ImVec4(0.22f, 0.34f, 0.50f, 1);
                s.Colors[ImGuiCol_Button] = ImVec4(0.13f, 0.16f, 0.21f, 1);
                s.Colors[ImGuiCol_ButtonHovered] = s.Colors[ImGuiCol_HeaderHovered];
                s.Colors[ImGuiCol_ButtonActive] = s.Colors[ImGuiCol_HeaderActive];
                s.Colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.17f, 0.22f, 1);
                s.Colors[ImGuiCol_FrameBgHovered] = s.Colors[ImGuiCol_HeaderHovered];
                s.Colors[ImGuiCol_FrameBgActive] = s.Colors[ImGuiCol_HeaderActive];
                s.Colors[ImGuiCol_CheckMark] = ImVec4(0.37f, 0.62f, 0.91f, 1);
                s.Colors[ImGuiCol_SliderGrab] = s.Colors[ImGuiCol_CheckMark];
                s.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.76f, 1, 1);
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
            page = Page::Home;
        if (page == Page::Options && (!category || std::find(submenu->m_Categories.begin(), submenu->m_Categories.end(), category) == submenu->m_Categories.end()))
            page = Page::Categories;

        auto& io = ImGui::GetIO();
        // Native ImGui keyboard navigation; controller input needs a backend mapping.
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        const ImVec2 size(std::min(460.0f, io.DisplaySize.x - 32.0f), std::min(680.0f, io.DisplaySize.y - 32.0f));
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(32, 32), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSizeConstraints(ImVec2(std::min(420.0f, size.x), std::min(400.0f, size.y)),
            ImVec2(io.DisplaySize.x - 16.0f, io.DisplaySize.y - 16.0f));
        UIManager::SetShowContentWindow(true);
        const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
        if (ImGui::Begin("##YimModernList", nullptr, flags))
        {
            ImGui::TextUnformatted("YIMMENU V2");
            ImGui::SameLine();
            const char* tag = "MODERN LIST";
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(tag).x - 16.0f);
            ImGui::TextDisabled("%s", tag);
            ImGui::Separator();
            ImGui::Spacing();
            const auto goBack = [&] {
                if (page == Page::Options) page = Page::Categories;
                else page = Page::Home;
            };
            if (page != Page::Home)
            {
                if (ImGui::Button("< Back", ImVec2(80, 36))) goBack();
                ImGui::SameLine();
            }
            std::string breadcrumb = "Home";
            if (page != Page::Home && submenu) breadcrumb = submenu->m_Name;
            if (page == Page::Options && category) breadcrumb += " / " + category->m_Name;
            ImGui::TextUnformatted(breadcrumb.c_str());
            if (!io.WantTextInput && !ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel)
                && ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)
                && ImGui::IsKeyPressed(ImGuiKey_Backspace, false)) goBack();

            ImGui::Spacing();
            // Distinct child identities keep scroll positions separate between routes.
            ImGui::PushID(page == Page::Home ? nullptr : submenu.get());
            ImGui::PushID(page == Page::Options ? category.get() : nullptr);
            const char* childId = page == Page::Home ? "##home-list" : page == Page::Categories ? "##category-list" : "##option-list";
            if (ImGui::BeginChild(childId, ImVec2(0, -40.0f), false, ImGuiWindowFlags_HorizontalScrollbar))
            {
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
            ImGui::TextDisabled("Arrows / Enter: navigate   Backspace: back");
        }
        ImGui::End();
    }
}
