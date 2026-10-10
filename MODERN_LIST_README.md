# Modern List — Keyboard v2 source update

## What this is

Updated YimMenuV2 **Enhanced source**, not a DLL. Apply this to the source used for the working first Modern List build. It is not a Legacy port and does not modify any uploaded binary.

The first Modern List revision was compiled and shown running in-game by the user. **This second revision has not been Windows-built or tested in GTA here.** Compile it and verify the checks below before relying on it.

## Changes

- Original code-drawn cyan/teal gradient YIMMENU banner, charcoal panel and one filled keyboard-selection bar. The supplied Impulse archive contains texture references but no header image files; no Impulse source, artwork, authentication or other features are included.
- Requests keyboard focus when opening Modern List and when navigating between Home, Categories and Options.
- No ImGui cursor in ordinary Modern List mode; no OS cursor repositioning on list open/close. The onboarding dialog and existing mouse layouts retain mouse input.
- Removes the previous blanket game-control suppression during ordinary list browsing. Only the six frontend navigation controls are suppressed; movement, camera, sprint/jump and driving inputs are not deliberately disabled by this browsing branch.
- Raw mouse buttons continue to the game while browsing the cursorless list.
- Space key events continue to the game but bypass ImGui while browsing, preventing jump/handbrake from also activating the focused menu command. Text/active widget editing is excepted.
- Focused command descriptions in the footer for common command widgets.
- Existing feature bindings retained. Complex custom editors are not rewritten; keyboard usability varies by editor. Controller navigation has not been added or verified.
- Fixed compact panel at the top left, with scrolling for longer lists and wide editors. Other themes retain their original layout behavior.

## Keys

- **Insert** or the existing **Ctrl + backslash** shortcut: open/close.
- **Up / Down**: move between navigable rows.
- **Enter**: open a section, toggle a Boolean, run an action, or activate an editor.
- **Right**: also opens a focused submenu/category row. Native widgets use their usual editing keys.
- **Backspace**: up one menu level, unless a text field, active widget or popup is handling input.
- **Left**: returns from the Categories page to Home. On the Options page, Left remains available for widgets/collapsing groups.
- **Tab / Shift+Tab**: alternate native ImGui focus navigation; these are not reserved exclusively from the game.
- **Escape**: use normal widget behavior; close the menu with Insert, not Escape.

**Text-entry exception:** the original game input block is deliberately retained while actively typing in a text/numeric editor. Ordinary row browsing should no longer freeze movement. Onboarding and other mouse layouts retain their existing input policy.

## Apply and build through GitHub Desktop / Actions

1. Extract the source update ZIP. Open its outer folder.
2. Copy the **contents** (including `src`, `docs` and `tests`) into your existing cloned YimMenuV2 repository folder. Replace matching files. Do not nest the outer folder inside the repository.
3. Commit the changes in GitHub Desktop and push to the same `enhanced` branch used for the successful first build. No pull request is needed for your own fork build.
4. Open your fork's Actions page. Wait for the new workflow to succeed.
5. Download and extract `RelWithDebInfo-Native-Clang`. Use the newly built DLL from that artifact, not the old first-revision DLL.

For a saved non-list theme, select Settings -> GUI -> UI -> UI Style -> Modern List. Existing saved style choices remain respected; new configurations default to Modern List.

The separate full-source ZIP contains the complete modified source tree, license and original build workflows. It is an alternative to applying the update ZIP, not another folder to merge underneath it.

## Verification and limitations

- Renderer compiled with GCC against mock ImGui/frontend types; route transitions, focus requests, Right-arrow activation, Backspace guards, removed submenu handling, balanced windows and style restoration tested.
- All 16 open/list/onboarding/text-edit combinations tested against the pure cursor/movement policy.
- Source diff checked and applied cleanly to a fresh extraction of the original uploaded Enhanced source, then compared byte-for-byte.
- Updated illustrative HTML reference rendered and inspected. It is not a screenshot from GTA or the compiled DLL.
- Full Windows compilation/linking, real ImGui integration, game input behavior, DPI changes, hotkey popups and all custom editors remain unverified for this revision.

After building, test: opening focuses a row; Up/Down and Enter navigate; Backspace returns; no list cursor is drawn; WASD/camera and Space work during browsing; typing intentionally captures controls; closing/switching themes restores normal behavior. Menu updates do not establish anti-cheat or account safety.

## Included

- `modern-list-ui.patch`: combined source diff against the original uploaded Enhanced ZIP.
- `docs/modern-list-preview.html` and `.png`: illustrative cyan-banner reference.
- `tests/modern-list`: optional isolated mock tests, not a Windows or game integration test.

Original project license retained. No DLL or compiled executable is included.
