# Screenshot-inspired Cherax Click layout — YimMenuV2 source

This revision adds a mouse-clickable ImGui layout inspired by the three supplied Cherax screenshots. It is NOT an exact, pixel-identical Cherax clone. Original YimMenu branding, tab names, commands and custom editors remain. It does not add the scripts, remote actions or capabilities pictured in another product. No Cherax code, proprietary textures or logo font is included.

## New interface
- Single draggable dark window with bottom color glow.
- Left icon rail for existing YimMenu submenus; hover tooltips identify each section.
- Mouse-clickable horizontal category tabs, framed panels and two-column card layout; narrower content falls back to one column.
- Square native ImGui checkboxes, compact buttons, full-width value widgets and dropdowns.
- Player search and name/ID sorting inside a dock, existing player selection, and a read-only status dock when screen space allows. The original full Info tab remains available.
- Top-right section-search field, theme icon and bookmark star. Search filters existing group/category titles, not every individual command.
- Favorites menu backed by bookmarks.txt. Opening a saved section navigates only; it does not run commands.
- Purple Glow, Red Glow and Monochrome presets with editable RGBA colors, window/card appearance, player-list width and texture intensity.

## Select the new layout
Settings > GUI > UI > UI Style > Cherax Click (Screenshots).
Numeric style ID 5 is new. Existing saved IDs 0–4 are preserved, so a previous installation may still show the old layout until you select the new one. New configurations default to the click layout.

Mouse controls operate the UI normally. Insert / the existing Ctrl + backslash shortcut still opens and closes the menu. Mouse wheel scrolls the ImGui content; it is not converted into a list-menu selection.

## Theme storage
Initialization creates `%APPDATA%\YimMenuV2\themes\click-ui\` independently of list-ui and themes.json.
Click the paintbrush icon or open Settings > Customize > Click Themes.
Load a preset, edit the live values, enter a name and press Save / overwrite theme. Selection is remembered through active.txt. Unsaved edits do not survive restarting. Refresh discovers newly added .theme files. Example presets are in docs/click-theme-examples.
Theme names must use 1–48 ASCII letters/digits/spaces/hyphens/underscores; no leading/trailing spaces or Windows reserved device names. RGBA values are 0–1; width 560–1280, height 420–1000, player dock width 160–280, rounding 0–16, texture intensity 0–0.25. Missing keys inherit the default. Invalid/oversize data is rejected without replacing the active in-memory theme. Writes are ordinary files, not a transactional database.

## Input/phone change
The phone/frontend native-control reservation now applies to both the list and click layouts while open, across all three PAD groups. Arrow/Enter/Backspace events are delivered to ImGui but not forwarded to GTA's window; click-layout mouse button/wheel events likewise remain with the UI. Existing game mouse-control suppression is retained for click layouts. This is a source-level intended fix, not an in-game verified guarantee. An already-open phone is not forcibly closed.

## Build
This ZIP is full source, not a DLL. Copy the CONTENTS of its YimMenuV2 folder into your existing cloned source repository, replacing matching files. Do not create another nested project folder. Commit, push, then use the included original GitHub Actions Windows workflow. Download the new RelWithDebInfo-Native-Clang artifact after a successful build. Source files are picked up by the original recursive CMake source glob.
Suggested commit: Add screenshot-inspired Cherax click UI and custom themes.

## Verification and limitations
PASS: new renderer, theme storage/editor and bookmarks compiled against mock ImGui/frontend types using GCC C++17 with warnings treated as errors. Routes, player dock calls, columns, card callbacks, empty/clipped windows, popup paths, stack/style restoration, theme validation, favorites, filtering and input-policy decisions exercised.
PASS: older list-renderer regression tests remain passing.
The docs HTML/PNG previews are illustrative design references with demonstration controls/data, NOT a render of the actual compiled ImGui menu. They do not establish native widget or in-game fidelity.
Windows compilation/linking, real ImGui integration, fonts at all DPIs, every original custom editor, game mouse capture and the actual phone behavior remain unverified in this Linux sandbox. Build and test before relying on it. No account/anti-cheat safety guarantee is made.

The screenshots show three different themes/pages and do not reveal every interaction. This revision follows their visible icon rail, tabs, card structure and color styles; it cannot recreate unseen scripts, asset backgrounds, proprietary logo typography or exact behavior from screenshots alone.

Original project license, gameplay code and build workflows are preserved. cherax-click-ui.patch is a text-source diff against the ORIGINAL uploaded YimMenuV2 RAR, not against the previous list-only ZIP. Earlier UI documents describe historical revisions; this file describes the click revision.
