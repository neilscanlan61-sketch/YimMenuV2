#pragma once
namespace YimMenu { struct Renderer {
static void RequestBackgroundReload() {}
static const std::string& BackgroundStatus() { static const std::string status="Mock renderer: GIF integration not exercised"; return status; }
static bool DrawAnimatedBackground(ImDrawList*,ImVec2,ImVec2,float,float) { return false; }
}; }
