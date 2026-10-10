# Cherax-inspired ImGui list UI — source update

This is an original dark/violet, Cherax-inspired list presentation for the supplied YimMenuV2 source. It is not an exact Cherax clone and contains no Cherax code, branding or assets. Existing command bindings and other UI layouts are retained.

## Changes
- Violet-accented charcoal panel, left-aligned YIMMENU header, selected-row accent, compact breadcrumbs and descriptions. All list colors and panel/row dimensions come from a dedicated custom theme.
- Phone activation and phone navigation/scroll controls are suppressed in all three GTA PAD groups while the list menu is open. Arrow/Enter/Backspace events reach ImGui first, then are not forwarded to GTA's window. These changes target the phone opening when you navigate sections.
- WASD, camera, sprint, jump/Space and driving remain available during ordinary keyboard list browsing. The original text-entry input capture is retained. Suppression ends when the menu closes or another layout is selected; an already-open phone is not forcibly closed.
- Automatically creates `%APPDATA%\YimMenuV2\themes\list-ui\` at UI initialization. Separate from the existing global `themes.json`.
- Three initial presets: Cherax Violet, Midnight Blue, Emerald. Existing files are never overwritten by initialization.
- Preset loading, refresh, live color/dimension editing, save/overwrite under a custom name, reload, and remembered selection via `active.txt`. Malformed, oversized and unsafe-name files are rejected without replacing the current in-memory theme.

## Build and install
1. Extract this ZIP. It contains the complete edited source, not a prebuilt DLL.
2. Merge the contents of `YimMenuV2` into your existing repository, replacing matching files. Do not put it in an extra nested `YimMenuV2` folder. Alternatively, build this complete source directly with the original toolchain.
3. Commit and push to the same branch/fork you already use to build this source. The included GitHub Actions workflow builds the Windows DLL; download the new `RelWithDebInfo-Native-Clang` artifact when successful.
4. Select `Settings > GUI > UI > UI Style > Cherax-inspired List` if you have another style saved. The existing numeric style ID (4) is preserved.
5. Open `Settings > Customize > List Themes` to manage the list presets. Arrow keys/Tab navigate widgets; Enter activates. The list stays keyboard-first, with no mouse cursor. Mouse-wheel browsing is not newly implemented by this update.

## Controls
Insert (or the existing Ctrl + backslash shortcut): open/close.
Up/Down: rows. Enter/Right: open section. Backspace: back. Space stays a gameplay input during ordinary browsing. Text editing and active popups keep normal ImGui handling.

## Custom files
Use the editor or copy a `.theme` file into the list-ui folder, then press Refresh theme list. Select a preset to load it; edits preview while the menu is running. Save / overwrite theme persists the changes and makes that preset the remembered selection. Unsaved live edits do not survive restarting.
Names: 1–48 ASCII letters/digits/spaces/hyphens/underscores; no leading/trailing spaces or Windows device names. RGBA channels are floats from 0 to 1. Width: 320–760, height: 420–1000, row height: 36–64, rounding: 0–16. Missing keys use the default violet theme values. Unknown keys, extra values and out-of-range data produce an error message. The panel is clamped to the available display size.

## Verification — read before use
PASS: real list-renderer source compiled against the provided mock ImGui/frontend harness with GCC C++17, warnings treated as errors; routing, focus, back guards, removed routes, style restoration and all 16 input-policy combinations.
PASS: real theme persistence code compiled and exercised for initialization, preset listing, save/load, process restart restoration, invalid paths/names, malformed/out-of-range/oversize data, and non-overwrite initialization.
PASS: phone-control set and key mapping tests, plus exclusions for movement/camera/jump/sprint.
The HTML/PNG reference is an illustrative design preview, NOT an in-game screenshot or render of the compiled ImGui menu.

Full Windows compilation/linking, the actual ImGui integration and GTA input behavior could not be tested in this Linux sandbox. There is no compiled DLL included. Build first, then verify the phone stays closed during Up/Down/Enter/Backspace navigation, controls return after closing, theme files appear and survive restarting, and custom editors remain usable. No anti-cheat/account-safety guarantee is made.

Original project license and build workflows are preserved. `cherax-list-ui.patch` is a diff against the source in the uploaded RAR. Existing older Modern List documentation is historical; this file describes the new revision.
