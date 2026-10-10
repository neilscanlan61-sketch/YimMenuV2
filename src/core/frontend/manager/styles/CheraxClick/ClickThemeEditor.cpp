#include "ClickTheme.hpp"
#include "core/renderer/Renderer.hpp"

namespace YimMenu::ClickUI
{
    void DrawThemeEditor()
    {
        static char custom[49] = "Rojo Animated";
        static std::vector<std::string> presets;
        static bool scanned = false;
        if (!scanned) { presets = ThemeNames(); scanned = true; }
        ImGui::TextWrapped("Theme and custom theme loading. Put .theme and matching .gif files in the folder below.");
        ImGui::TextUnformatted("Theme");
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##theme-preset", ActiveThemeName().c_str()))
        {
            for (const auto& preset : presets)
                if (ImGui::Selectable(preset.c_str(), preset == ActiveThemeName()))
                    if (LoadTheme(preset)) Renderer::RequestBackgroundReload();
            ImGui::EndCombo();
        }
        ImGui::TextUnformatted("Custom theme name");
        ImGui::SetNextItemWidth(-1);
        ImGui::InputText("##custom-theme-name", custom, sizeof(custom));
        if (ImGui::Button("Load custom theme"))
            if (LoadTheme(custom)) Renderer::RequestBackgroundReload();
        if (ImGui::Button("Refresh themes / GIF"))
        {
            presets = ThemeNames(); Renderer::RequestBackgroundReload();
        }
        bool animated = CurrentTheme().BackgroundEnabled >= 0.5f;
        if (ImGui::Checkbox("Animated GIF background", &animated))
        {
            CurrentTheme().BackgroundEnabled = animated ? 1.0f : 0.0f;
            SaveTheme(ActiveThemeName()); Renderer::RequestBackgroundReload();
        }
        ImGui::TextWrapped("GIF: <theme name>.gif, or background.gif. Turn animation off for reduced motion. Size, colors and light strength can be edited in the .theme file.");
        ImGui::TextWrapped("Folder: %s", ThemeFolder().string().c_str());
        if (!ThemeStatus().empty()) ImGui::TextWrapped("%s", ThemeStatus().c_str());
        ImGui::TextWrapped("%s", Renderer::BackgroundStatus().c_str());
    }
}
