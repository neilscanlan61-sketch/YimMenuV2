# Rockstar Tool — mini UI and Teleport rework

This update builds on the supplied Rockstar Tool compact/GIF source revision.
Original YimMenuV2 attribution and LICENSE are preserved. Not affiliated with Rockstar Games or Cherax.
This is a SOURCE package, not a compiled DLL or an exact Cherax implementation.

## What changed

- New **Rockstar Mini** preset: **640 x 500** main window, 160 px player pane.
  Existing themes/active selection are preserved; select Rockstar Mini to use the new size.
- Rounded command buttons, input frames, sliders, checkboxes and category buttons.
- Softer, static accent light pools behind the interface; real GIF playback remains supported.
- Smaller sidebar buttons and category strip; controls still scroll when needed.
- Teleport tabs: **Quick Travel**, **Directional**, **Saved**.
- Saved locations: category-scoped search, select + Teleport, double-click teleport,
  a Save-current-location dialog and explicit delete confirmation (no Shift-delete).
- Save/delete/teleport fibers capture names, category and location values instead of mutable UI state.
- Saved-location renderers use mutex-protected snapshots. A pre-existing dangling-pointer
  lookup in the location backend was corrected; legacy reference/pointer APIs remain
  for source compatibility and must not be retained across mutations.
- Vehicle > Main > Tools: **Convert Current Vehicle to PV**, bound to the existing
  savepersonalvehicle command. Its original session/vehicle/blacklist checks remain.
  This does not guarantee that every vehicle can become a personal vehicle.
- Theme window simplified to theme selection, custom-theme-name loading, Refresh themes / GIF,
  and an animated-background switch. Removed the color, size, rounding and speed sliders
  and manual theme-save controls from this window. Advanced values remain editable in .theme files.
- Included Rockstar Mini and Rojo Mini animated presets using the previously supplied GIF.

**Not included:** Free Shopping. The uploaded patch does not provide a verified Free Shopping
implementation; no dummy toggle or promise that online purchases are free has been added.
Recovery is otherwise unchanged.

## Install

1. Build the source using the project's existing Windows/GitHub build requirements.
   The CMake target stays YimMenuV2 and the DLL output stays RockstarTool.dll.
2. Select UI Style > Rockstar Tool (Compact Click).
3. Copy these files from docs/rockstar-tool into
   `%APPDATA%\YimMenuV2\themes\click-ui\`:
   - Rockstar Mini.theme + Rockstar Mini.gif
   - Rojo Mini.theme + Rojo Mini.gif
4. Open the paintbrush/theme window, click **Refresh themes / GIF**, then choose
   **Rockstar Mini**. Existing saved presets are not overwritten automatically.
5. For a custom theme, copy `My Theme.theme` and `My Theme.gif` into the same folder,
   enter `My Theme` (without an extension) and click **Load custom theme**.
6. Enable Animated GIF background. The checkbox persists into the active .theme file.
   Refresh after replacing a GIF with the same name. GIF motion pauses while the menu is closed.

GIF lookup is `<active theme name>.gif`, falling back to background.gif. Only the main click
window is animated; toast/header images and Cherax's own custom theme format are not supported.
Missing/unsupported GIFs fall back to the color background. Existing animation limits and
D3D12 fencing are retained. GIF animation can be switched off for reduced motion.

Custom .theme fields include Width, Height, Rounding, LightIntensity (0..0.6),
BackgroundEnabled (0 or 1), BackgroundOpacity (0..1), BackgroundSpeed (0.1..3).
A LightIntensity of 0 disables the static background lights. The complete color/schema format
is shown in the included presets. Old compatible click .theme files still load.

## Validation and limitations

PASS: click renderer/theme/bookmark/input regression with real UI code against mocks.
PASS: isolated real Teleport.cpp routing tests, selection, deferred copied values,
vehicle-position saving, duplicate-name policy, invalid names and confirmed deletion.
PASS: portable GIF timing/compositing/disposal tests and existing Savage camera policy tests.
PASS: custom theme parser round-trip and incremental patch/source ZIP checks.

Preview HTML/PNGs are illustrative designs, NOT compiled Windows/DLL screenshots.
The tests do not exercise GTA-native actions, real saved-location disk persistence,
Windows WIC decode, D3D12 uploads or a complete Windows build. Those need testing after building.
The existing Savage guard remains a workaround, not a confirmed root-cause fix.
The website/license portal is separate and has not been integrated into this mod.

## Patch

mini-ui-teleport-rework.patch applies to the exact supplied previous Rockstar Tool source
revision, not to a random upstream checkout. The full ZIP can be used instead of patching.
Do not apply this patch twice. Old update patches in this package are historical.
