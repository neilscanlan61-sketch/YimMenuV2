# YimMenuV2 — Modern List UI source modification

## Deliverable and status

This is modified **source**, not a rebuilt DLL. It targets the supplied YimMenuV2-enhanced.zip. It does not modify or replace the previously supplied YimMenu.dll and should not be assumed compatible with that older binary.

The new layout is inspired by compact vertical list menus; it does not copy Paragon/Rebound branding, artwork, or proprietary code.

## UI changes

- New `Modern List` renderer, appended as style value 4 without renumbering existing styles.
- Default style is Modern List for new configurations. Saved style choices remain respected.
- Home -> submenu categories -> options navigation with a Back button and breadcrumbs.
- Dark panel, blue highlights, clipped long row labels, and trailing arrows.
- Action buttons become full-width RUN rows.
- Boolean controls become full-width ON/OFF rows.
- Integer, float and list command controls retain their original command bindings, with labels at left and editable controls at right.
- Named groups become expanding sections, without the previous multi-column arrangement.
- TabBarItem/TabItem content becomes expanding vertical sections in this layout.
- Keyboard navigation uses ImGui's existing arrow/Tab/Enter support; Backspace goes up a level when no popup/text editor is active. Controller navigation is not added or verified.
- Route-specific scroll identities and checks for removed dynamic submenus/categories.
- Fixed list palette is restored after each render, leaving other layouts/overlays unaffected by the list renderer.
- Custom ImGuiItem callbacks and complex editors retain their original controls rather than being rewritten. The panel is resizable and allows horizontal scrolling where those editors need extra space.
- Existing feature definitions and game/backend logic remain unchanged. GUI input suppression is extended to prevent list keyboard navigation from also controlling the game. A queued Button callback is copied to avoid capturing the UI object's lifetime.

## Selecting it

After building and opening the menu, select **Settings -> GUI -> UI -> UI Style -> Modern List** if an existing settings file restores a different style. The existing menu-opening shortcut is unchanged.

## Windows build

Use the project's existing build prerequisites: Git, CMake 3.20 or newer, Ninja, Visual Studio C++ build tools/Windows SDK, and Clang for Windows. Internet access is required for the project's FetchContent dependencies.

From an x64 Visual Studio developer terminal in this directory, following the repository's native CI configuration:

```powershell
cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build --config RelWithDebInfo --target YimMenuV2
```

The expected native build output is `build/YimMenuV2.dll`. These instructions follow the uploaded source's workflow; the build was not run successfully in this environment. Test the resulting build in an appropriate permitted test environment before relying on it.

## Verification performed

- The new renderer was compiled with GCC against a mocked ImGui/frontend harness and its navigation tests passed: Home/category/option transitions, Backspace, popup guard, removed submenu fallback, balanced Begin/End calls, and style restoration.
- Patch integration and file scope checked against the original uploaded ZIP.
- The HTML design reference was rendered and inspected at desktop and narrow widths. It is not a production ImGui render or an in-game screenshot.

## Not verified

- Complete project compilation/linking against the pinned, real ImGui and Windows dependencies.
- In-game rendering, all custom editors, command interactions, hotkey popups, DPI scaling, keyboard focus and game input integration.
- Compatibility with future repository versions or the separately uploaded DLL.

No DLL is included. Do not rename this ZIP, a source file, or a patch to `.dll`.

## Included reference and patch

- `modern-list-ui.patch`: source diff against the uploaded ZIP.
- `docs/modern-list-preview.html`: illustrative layout reference (no live game functions).
- `docs/modern-list-preview.png`: inspected desktop reference image.

The original project license remains included. Modified source retains that license.
