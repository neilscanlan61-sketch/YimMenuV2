#pragma once

namespace YimMenu
{
    struct GUIInputPolicy
    {
        bool KeyboardList;
        bool MouseUI;
        bool PreserveGameplay;
    };

    // No platform/API dependencies: keeps the movement/cursor decision testable.
    constexpr GUIInputPolicy GetGUIInputPolicy(bool open, bool listStyle, bool onboarding, bool textEditing)
    {
        const bool keyboardList = open && listStyle && !onboarding;
        return {keyboardList, onboarding || (open && !keyboardList), keyboardList && !textEditing};
    }
}
