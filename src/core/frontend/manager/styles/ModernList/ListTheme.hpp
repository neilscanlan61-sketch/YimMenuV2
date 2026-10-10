#pragma once
#include <imgui.h>
#include <filesystem>
#include <string>
#include <vector>

namespace YimMenu::ListUI
{
    struct Theme
    {
        ImVec4 Background{0.055f, 0.055f, 0.075f, 0.98f};
        ImVec4 Surface{0.080f, 0.080f, 0.105f, 1.0f};
        ImVec4 Accent{0.69f, 0.51f, 1.0f, 1.0f};
        ImVec4 Selection{0.19f, 0.14f, 0.29f, 1.0f};
        ImVec4 Text{0.95f, 0.94f, 0.98f, 1.0f};
        ImVec4 Muted{0.69f, 0.67f, 0.75f, 1.0f};
        ImVec4 Border{0.24f, 0.22f, 0.31f, 1.0f};
        ImVec4 Header{0.105f, 0.08f, 0.165f, 1.0f};
        float Width = 440.0f;
        float Height = 700.0f;
        float RowHeight = 44.0f;
        float Rounding = 8.0f;
    };
    Theme& CurrentTheme();
    // Initialize once from the project's configured data directory, not the DLL directory.
    void InitializeThemes(const std::filesystem::path& folder);
    const std::filesystem::path& ThemeFolder();
    std::vector<std::string> ThemeNames();
    bool LoadTheme(const std::string& name);
    bool SaveTheme(const std::string& name);
    const std::string& ThemeStatus();
    const std::string& ActiveThemeName();
    void DrawThemeEditor();
}
