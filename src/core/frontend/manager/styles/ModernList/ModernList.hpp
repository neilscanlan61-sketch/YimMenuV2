#pragma once

namespace YimMenu::ListUI
{
    // Scoped to the Modern List renderer, so changing styles mid-frame is safe.
    bool Active();
    bool Row(const void* id, const char* label, const char* value = ">", bool selected = false);
    void BeginValue(const char* label, float width = 168.0f);
}
