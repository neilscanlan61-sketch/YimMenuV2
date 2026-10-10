#pragma once
#include <array>
#include <cstdint>
#include "types/pad/ControllerInputs.hpp"

namespace YimMenu
{
    inline constexpr std::array ListInputControls = {
        ControllerInputs::INPUT_PHONE,
        ControllerInputs::INPUT_CELLPHONE_UP, ControllerInputs::INPUT_CELLPHONE_DOWN,
        ControllerInputs::INPUT_CELLPHONE_LEFT, ControllerInputs::INPUT_CELLPHONE_RIGHT,
        ControllerInputs::INPUT_CELLPHONE_SELECT, ControllerInputs::INPUT_CELLPHONE_CANCEL,
        ControllerInputs::INPUT_CELLPHONE_OPTION, ControllerInputs::INPUT_CELLPHONE_EXTRA_OPTION,
        ControllerInputs::INPUT_CELLPHONE_SCROLL_FORWARD, ControllerInputs::INPUT_CELLPHONE_SCROLL_BACKWARD,
        ControllerInputs::INPUT_FRONTEND_UP, ControllerInputs::INPUT_FRONTEND_DOWN,
        ControllerInputs::INPUT_FRONTEND_LEFT, ControllerInputs::INPUT_FRONTEND_RIGHT,
        ControllerInputs::INPUT_FRONTEND_ACCEPT, ControllerInputs::INPUT_FRONTEND_CANCEL};
    // Win32 virtual-key values without depending on windows.h.
    constexpr bool IsListNavigationKey(std::uintptr_t key)
    {
        return key == 0x26 || key == 0x28 || key == 0x25 || key == 0x27 // arrows
            || key == 0x0D || key == 0x08; // Enter / Backspace
    }
}
