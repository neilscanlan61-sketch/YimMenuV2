#include "Mock.hpp"
#include "../../src/core/frontend/manager/styles/ModernList/ListTheme.cpp"
#include "../../src/game/frontend/ListInputControls.hpp"
#include "../../src/game/frontend/GUIInputPolicy.hpp"
#include <set>
int main(int argc, char** argv)
{
    using namespace YimMenu;
    using namespace YimMenu::ListUI;
    assert(argc == 2);
    const std::filesystem::path folder(argv[1]);
    InitializeThemes(folder);
    if (std::string(argv[1]).find("restart") != std::string::npos)
    {
        assert(ActiveThemeName() == "My Test");
        assert(std::abs(CurrentTheme().Width - 512) < 0.01f);
        std::cout << "PASS: saved preset restored across process restart.\n"; return 0;
    }
    assert(std::filesystem::is_directory(folder));
    assert(ThemeNames().size() == 3);
    assert(ActiveThemeName() == "Cherax Violet");
    CurrentTheme().Width = 512; CurrentTheme().Accent = ImVec4(.2f, .7f, .9f, 1);
    assert(SaveTheme("My Test"));
    CurrentTheme().Width = 600;
    assert(LoadTheme("My Test")); assert(CurrentTheme().Width == 512);
    assert(ThemeNames().size() == 4);
    for (const auto* name : {"../escape", "a/b", "", "x.", "CON", "LPT1", "a\\b", " bad", "bad "})
        assert(!SaveTheme(name));
    assert(!LoadTheme("missing")); assert(CurrentTheme().Width == 512);
    for (const auto* content : {"Accent 1 2 3 4", "Width 2", "Height nan", "Unknown 1", "Text 1 1", "Rounding 2 extra"})
    {
        std::ofstream(folder / "Bad.theme") << content;
        assert(!LoadTheme("Bad")); assert(CurrentTheme().Width == 512);
    }
    std::ofstream(folder / "Huge.theme") << std::string(17000, 'x');
    assert(!LoadTheme("Huge"));
    assert(LoadTheme("Midnight Blue"));
    assert(LoadTheme("My Test"));
    // Never overwrite existing preset files during initialize.
    const auto size = std::filesystem::file_size(folder / "My Test.theme");
    InitializeThemes(folder); assert(std::filesystem::file_size(folder / "My Test.theme") == size);
    std::set<int> unique;
    for (auto control : ListInputControls) assert(unique.insert(static_cast<int>(control)).second);
    assert(unique.count(static_cast<int>(ControllerInputs::INPUT_PHONE)));
    assert(unique.count(static_cast<int>(ControllerInputs::INPUT_CELLPHONE_SCROLL_BACKWARD)));
    for (auto control : {ControllerInputs::INPUT_MOVE_LR, ControllerInputs::INPUT_MOVE_UD,
        ControllerInputs::INPUT_JUMP, ControllerInputs::INPUT_SPRINT, ControllerInputs::INPUT_LOOK_LR})
        assert(!unique.count(static_cast<int>(control)));
    for (auto key : {0x25,0x26,0x27,0x28,0x0d,0x08}) assert(IsListNavigationKey(key));
    for (auto key : {0x20,0x41,0x57,0x2d,0x09}) assert(!IsListNavigationKey(key));
    std::cout << "PASS: theme initialization/list/save/load, path validation, malformed/oversize rejection, gameplay exclusions and navigation keys.\n";
}
