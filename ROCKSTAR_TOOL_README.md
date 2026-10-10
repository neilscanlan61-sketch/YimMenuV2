# Rockstar Tool — compact click UI and animated GIF backgrounds

This is a user-requested UI fork of YimMenuV2. It is not affiliated with,
endorsed by or an official product of Rockstar Games. Original project
namespaces, source attribution, copyright and LICENSE are retained.

## What changed

- Visible click-menu header: ROCKSTAR TOOL.
- Style 5 is now labeled **Rockstar Tool (Compact Click)**; existing numeric IDs stay intact.
- Default click-theme dimensions: 720 × 560 instead of 860 × 740; player pane 180 px.
- A new **Rockstar Compact** preset. Existing themes and active selection are not overwritten.
- Header text stays in its own area, separate from search/theme/bookmark controls.
- Animated GIF background decoding through Windows Imaging Component (WIC),
  then playback through one D3D12 texture with per-backbuffer staging buffers.
- GIF controls: enable/disable, opacity, speed, reload; a **Use compact size** shortcut.
- Existing mouse controls, phone-input filtering, themes, favorites and Lua APIs remain.
- Vehicle > Main > Misc now has **Savage Entry Camera Guard**, enabled by default
  unless the saved command configuration overrides it.
- While the click menu is open, the view-cycle control is reserved so browsing does
  not also cycle the game camera. Normal gameplay camera input remains available when closed.
- DLL output is named RockstarTool.dll (libRockstarTool.dll for cross-compile output);
  GitHub build artifact paths were updated without renaming the CMake build target.

## Build / apply

This package is SOURCE, not a compiled DLL. Use the original Windows build
requirements and the included GitHub workflows. The CMake target remains
YimMenuV2 for compatibility. Windows SDK libraries windowscodecs and ole32
are linked for WIC and COM; there is no new third-party decoder download.

The incremental rockstar-tool-update.patch applies to the exact earlier
Cherax Click source package supplied in this chat. Do not stack it on an
unrelated version or on the original unmodified RAR.

## Install the supplied animated presets

1. Build the updated DLL and use **UI Style > Rockstar Tool (Compact Click)**.
2. Copy the contents of docs/rockstar-tool/ listed below into:
   `%APPDATA%\YimMenuV2\themes\click-ui\`
   - Rockstar Compact.theme
   - Rockstar Compact.gif
   - Rojo Animated.theme
   - Rojo Animated.gif
3. Open the paintbrush icon (or Customize > Click Themes).
4. Click **Refresh theme list**, then choose **Rockstar Compact** or **Rojo Animated**.
5. Enable **Animated GIF background** if disabled; use **Reload GIF** after replacing the file.
6. Save/overwrite the theme to persist changes to enable, opacity, speed or dimensions.

GIF lookup is `<active preset name>.gif`, with `background.gif` as fallback
in that same folder. Existing .theme files remain readable; missing animation
fields default to disabled. Choosing a preset saves the active selection.
No settings/data-folder migration: old Lua scripts and configuration stay under YimMenuV2.

Animation pauses while the menu is closed, onboarding is active or the animation
switch is off. Disable it for reduced motion. Only the click-layout MAIN window
background is animated; toast backgrounds and Cherax header images are not implemented.
There is a center-cropped cover fit so the image is not stretched.
Missing, malformed or unsupported images fall back to the color background.
GIF limits: 16 MiB compressed, 1024 × 1024 maximum, 180 frames, 64 MiB of decoded RGBA.
Very short delays are normalized to 20 ms. The animation loops while enabled.
Disposal 2 clears to transparent over the menu base; rare opaque-background GIFs
may consequently look different from a browser. The supplied 33-frame, 480 × 480
GIF uses retained opaque frames and fits within these limits.

## Savage camera workaround — not a confirmed root-cause fix

No explicit forced first-person assignment was found in the inspected original
vehicle feature code. The symptom may relate to GTA camera preferences/context
or another feature/script. This update adds a bounded workaround:

- Only the Savage (`savage`) is considered.
- During the first 1.5 seconds after entering, an active normal vehicle camera
  found in first person is moved to the remembered third-person distance once.
- Manual camera cycling cancels the entry guard; no continuous first-person lock.
- No correction after the timeout or in unrelated vehicles/scripted camera views.
- Disable **Savage Entry Camera Guard** in Vehicle > Main > Misc to retain
  first-person entry. Toggle commands can be persisted by the existing settings system.

Actual Savage-entry behavior must be tested in-game, including aiming and exiting.

## Validation performed and limits

PASS: isolated C++ click layout/theme regression with mock ImGui/frontend.
PASS: real theme parser with new animation-field validation and existing-file compatibility.
PASS: portable GIF timing, loops, bounds, transparency and disposal 2/3 compositor tests.
PASS: portable Savage entry policy (one-shot, cancellation, timeout, timer wrap).
PASS: incremental patch applied to the earlier source and byte-for-byte comparison.
PASS: source ZIP integrity.

Illustrative HTML previews show the supplied GIF and compact design but are NOT
DLL screenshots. WIC decode, D3D12 resource uploads/lifecycle, Windows compilation
and actual game/camera integration could not be validated in this Linux sandbox.
The mock tests do not make those untested paths production-ready or prove the
Savage symptom is fixed. Test the Windows build and game integration before relying on it.
