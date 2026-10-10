#pragma once

namespace YimMenu::ListUI
{
    bool Active();
    void RequestFocus();
    // Publishes a description for the keyboard-focused command in the footer.
    void Describe(const char* description);
    bool Row(const void* id, const char* label, const char* value = ">", bool enabled = false);
    void BeginValue(const char* label, float width = 168.0f);
}
