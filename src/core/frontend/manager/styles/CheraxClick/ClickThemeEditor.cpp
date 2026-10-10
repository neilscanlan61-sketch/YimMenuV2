#include "ClickTheme.hpp"
#include <cstring>

namespace YimMenu::ClickUI
{
    void DrawThemeEditor()
    {
        static char name[49] = "My Theme";
        static std::vector<std::string> presets;
        static bool scanned = false;
        if (!scanned) { presets = ThemeNames(); scanned = true; }
        ImGui::TextWrapped("Click-interface themes. Other layouts keep their own style. Changes preview immediately; save to keep them.");
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
        ImGui::ColorEdit4("Bottom glow", &theme.BackgroundLow.x);
        ImGui::ColorEdit4("Buttons", &theme.Button.x);
        ImGui::ColorEdit4("Hovered buttons", &theme.ButtonHover.x);
        ImGui::SliderFloat("Panel texture", &theme.Scanlines, 0, 0.25f, "%.2f");
        ImGui::SliderFloat("Player list width", &theme.PlayerWidth, 160, 280, "%.0f");
        ImGui::SliderFloat("Panel width", &theme.Width, 560, 1280, "%.0f");
        ImGui::SliderFloat("Panel height", &theme.Height, 420, 1000, "%.0f");
        ImGui::SliderFloat("Corner rounding", &theme.Rounding, 0, 16, "%.0f");
    }
}
