#include "ListTheme.hpp"
#include <cstring>

namespace YimMenu::ListUI
{
    void DrawThemeEditor()
    {
        static char name[49] = "My Theme";
        static std::vector<std::string> presets;
        static bool scanned = false;
        if (!scanned) { presets = ThemeNames(); scanned = true; }
        ImGui::TextWrapped("List-only themes. Other ImGui layouts keep their own style. Changes preview immediately; save to keep them.");
        ImGui::TextWrapped("Folder: %s", ThemeFolder().string().c_str());
        if (ImGui::BeginCombo("Preset", ActiveThemeName().c_str()))
        {
            for (const auto& preset : presets)
                if (ImGui::Selectable(preset.c_str(), preset == ActiveThemeName())) LoadTheme(preset);
            ImGui::EndCombo();
        }
        if (ImGui::Button("Refresh theme list")) presets = ThemeNames();
        ImGui::InputText("Theme name", name, sizeof(name));
        if (ImGui::Button("Save / overwrite theme"))
        {
            SaveTheme(name); presets = ThemeNames();
        }
        if (ImGui::Button("Reload saved theme")) LoadTheme(ActiveThemeName());
        if (!ThemeStatus().empty()) ImGui::TextWrapped("%s", ThemeStatus().c_str());
        ImGui::Separator();
        auto& theme = CurrentTheme();
        ImGui::ColorEdit4("Background", &theme.Background.x);
        ImGui::ColorEdit4("Surface", &theme.Surface.x);
        ImGui::ColorEdit4("Accent", &theme.Accent.x);
        ImGui::ColorEdit4("Selected row", &theme.Selection.x);
        ImGui::ColorEdit4("Text", &theme.Text.x);
        ImGui::ColorEdit4("Secondary text", &theme.Muted.x);
        ImGui::ColorEdit4("Border", &theme.Border.x);
        ImGui::ColorEdit4("Header", &theme.Header.x);
        ImGui::SliderFloat("Panel width", &theme.Width, 320, 760, "%.0f");
        ImGui::SliderFloat("Panel height", &theme.Height, 420, 1000, "%.0f");
        ImGui::SliderFloat("Row height", &theme.RowHeight, 36, 64, "%.0f");
        ImGui::SliderFloat("Corner rounding", &theme.Rounding, 0, 16, "%.0f");
    }
}
