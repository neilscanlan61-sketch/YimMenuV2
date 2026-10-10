> Latest UI revision: see ROCKSTAR_TOOL_REWORK_README.md for the 640 x 500 preset, Teleport rework and simpler theme/GIF loader.

> **Latest UI revision: Rockstar Tool (Compact Click)** — see [ROCKSTAR_TOOL_README.md](ROCKSTAR_TOOL_README.md) for GIF setup, compact presets and the Savage camera workaround. Source-only; Windows/game integration is unverified. This YimMenuV2 fork is not an official Rockstar Games product.

> **Latest UI revision:** See [CHERAX_CLICK_README.md](CHERAX_CLICK_README.md) for the mouse-clickable screenshot-inspired interface, preset folder, build steps and limitations. Source only; no DLL included.

> **UI update:** See [CHERAX_LIST_README.md](CHERAX_LIST_README.md) for the Cherax-inspired list UI, phone-input changes, custom theme folder, build steps and testing limitations. This archive contains source only, not a compiled DLL.

# YimMenuV2

Experimental menu for GTA 5: Enhanced

## How to use

1. Download the latest version of FSL from [here](https://www.unknowncheats.me/forum/grand-theft-auto-v/616977-fsl-local-gtao-saves.html) and place WINMM.dll in your GTA V directory. Using FSL is now optional but highly recommended for account safety
2. Download YimMenuV2 from [GitHub Releases](https://github.com/YimMenu/YimMenuV2/releases/tag/nightly)
3. Download an injector, such as [Xenos](https://www.unknowncheats.me/forum/general-programming-and-reversing/124013-xenos-injector-v2-3-2-a.html)
4. Open Rockstar Launcher, select Grand Theft Auto V Enhanced, go to settings, and disable BattlEye. If you are using Steam or Epic Games, you may have to pass the -nobattleye command line parameter as well
5. Launch GTA V, then use your injector to inject YimMenuV2.dll at the main menu

## How to open the menu?

Press the `INSERT` key or `Ctrl+\` to open the menu
 
## Common issues

### I keep getting desynced from public sessions every five minutes

We currently do not have a BattlEye bypass, and legitimate hosts will eventually remove you due to a heartbeat failure. There is currently no way to stop this other than using an actual (private) bypass

### I removed FSL and all my progress disappeared!

FSL reroutes account save data to disk, so any progress made with FSL will only show up if you have FSL enabled. If you don't want this, you can also use YimMenuV2 without FSL, but this is not recommended

### I removed FSL and the game doesn't start up anymore

This is a known issue; delete "Documents/GTAV Enhanced/Profiles" to fix
Trigger Modern List build.
