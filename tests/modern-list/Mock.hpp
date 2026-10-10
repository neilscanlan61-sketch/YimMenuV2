
#pragma once
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <cassert>
#include <iostream>
using ImU32=unsigned;using ImGuiWindowFlags=int;
struct ImVec2{float x=0,y=0;ImVec2(float a=0,float b=0):x(a),y(b){}};
struct ImVec4{float x=0,y=0,z=0,w=0;ImVec4(float a=0,float b=0,float c=0,float d=0):x(a),y(b),z(c),w(d){}};
enum {ImGuiCol_WindowBg,ImGuiCol_ChildBg,ImGuiCol_PopupBg,ImGuiCol_Text,ImGuiCol_TextDisabled,ImGuiCol_Border,ImGuiCol_Header,ImGuiCol_HeaderHovered,ImGuiCol_HeaderActive,ImGuiCol_Button,ImGuiCol_ButtonHovered,ImGuiCol_ButtonActive,ImGuiCol_FrameBg,ImGuiCol_FrameBgHovered,ImGuiCol_FrameBgActive,ImGuiCol_CheckMark,ImGuiCol_SliderGrab,ImGuiCol_SliderGrabActive,ImGuiCol_NavHighlight};
enum {ImGuiConfigFlags_NavEnableKeyboard=1,ImGuiConfigFlags_NavNoCaptureKeyboard=2,ImGuiCond_Always=2,ImGuiWindowFlags_NoResize=16,ImGuiWindowFlags_NoMove=32,ImGuiKey_RightArrow=2,ImGuiKey_LeftArrow=3,ImGuiCond_FirstUseEver=1,ImGuiWindowFlags_NoTitleBar=1,ImGuiWindowFlags_NoCollapse=2,ImGuiWindowFlags_NoScrollbar=4,ImGuiWindowFlags_HorizontalScrollbar=8,ImGuiPopupFlags_AnyPopupId=1,ImGuiPopupFlags_AnyPopupLevel=2,ImGuiFocusedFlags_RootAndChildWindows=1,ImGuiKey_Backspace=1};
#define IM_COL32(R,G,B,A) ((ImU32)(R))
struct ImFont{};
struct ImDrawList{void AddRectFilledMultiColor(ImVec2,ImVec2,ImU32,ImU32,ImU32,ImU32){} void AddText(ImFont*,float,ImVec2,ImU32,const char*){}void AddRectFilled(ImVec2,ImVec2,ImU32,float=0){} void PushClipRect(ImVec2,ImVec2,bool){}void PopClipRect(){}void AddText(ImVec2,ImU32,const char*){}};
struct ImGuiStyle{ImVec2 WindowPadding,ItemSpacing,FramePadding;float WindowRounding=3,ChildRounding=0,FrameRounding=0,WindowBorderSize=0,ScrollbarSize=0;ImVec4 Colors[30];};
struct ImGuiIO{ImVec2 DisplaySize{1920,1080};int ConfigFlags=0;bool WantTextInput=false;};
namespace ImGui{
inline ImGuiStyle style;inline ImGuiIO io;inline ImDrawList draw;inline const void* id=nullptr;inline const void* click=nullptr;inline bool back=false, key=false,popup=false,begin=true,child=true;inline const void* focused=nullptr;inline int nextKey=0;inline bool active=false;inline int focusRequests=0;inline int categoryDraws=0;inline int windowBalance=0,childBalance=0;
inline ImGuiStyle& GetStyle(){return style;}inline ImGuiIO& GetIO(){return io;}
inline void PushID(const void* p){id=p;}inline void PopID(){}inline ImVec2 GetCursorScreenPos(){return{16,16};}inline ImVec2 GetContentRegionAvail(){return{428,500};}inline float GetTextLineHeight(){return 19;}inline bool Selectable(const char*,bool,int,ImVec2){if(id==click&&click){click=nullptr;return true;}return false;}
inline ImDrawList* GetWindowDrawList(){return &draw;}inline bool IsItemHovered(){return false;}inline bool IsItemFocused(){return id==focused&&focused;}inline bool IsItemActive(){return active;}inline bool IsAnyItemActive(){return active;}inline void PushStyleColor(int,ImVec4){}inline void PopStyleColor(){}inline float GetFontSize(){return 19;}inline ImFont* GetFont(){static ImFont f;return &f;}inline void Dummy(ImVec2){}inline void SetNextWindowFocus(){++focusRequests;}inline void SetKeyboardFocusHere(int){++focusRequests;}template<class...T>void TextWrapped(const char*,T...){}inline ImVec2 CalcTextSize(const char* s){return{float(std::string(s).size()*9),19};}inline ImU32 GetColorU32(int){return 1;}inline ImU32 GetColorU32(const ImVec4&){return 2;}inline float GetFrameHeight(){return 39;}inline void SetCursorScreenPos(ImVec2){}inline void SetNextItemWidth(float){}
inline void SetNextWindowSize(ImVec2,int){}inline void SetNextWindowPos(ImVec2,int){}inline void SetNextWindowSizeConstraints(ImVec2,ImVec2){}inline bool Begin(const char*,bool*,int){++windowBalance;return begin;}inline void End(){--windowBalance;}inline void TextUnformatted(const char*){}inline void SameLine(){}inline void SetCursorPosX(float){}inline float GetWindowWidth(){return 460;}template<class...T>void TextDisabled(const char*,T...){}inline void Separator(){}inline void Spacing(){}
inline bool Button(const char*,ImVec2){bool a=back;back=false;return a;}inline bool IsPopupOpen(const char*,int){return popup;}inline bool IsWindowFocused(int){return true;}inline bool IsKeyPressed(int k,bool){if(k==nextKey&&nextKey){nextKey=0;return true;}if(k==ImGuiKey_Backspace){bool a=key;key=false;return a;}return false;}inline bool BeginChild(const char*,ImVec2,bool,int=0){++childBalance;return child;}inline void EndChild(){--childBalance;}inline void PushFont(ImFont*){}inline void PopFont(){}
}
namespace ImGui {
inline bool BeginCombo(const char*,const char*){return false;}
inline void EndCombo(){}
inline bool Selectable(const char*,bool){return false;}
inline bool Button(const char*){return false;}
inline bool InputText(const char*,char*,size_t){return false;}
inline bool ColorEdit4(const char*,float*){return false;}
inline bool SliderFloat(const char*,float*,float,float,const char*){return false;}
}
namespace YimMenu{
struct Category{std::string m_Name="Main";void Draw(){++ImGui::categoryDraws;}};
struct Submenu{std::string m_Name="Self";std::vector<std::shared_ptr<Category>>m_Categories;void SetActiveCategory(std::shared_ptr<Category>) {}};
struct UIManager{static inline std::vector<std::shared_ptr<Submenu>>subs;static const auto& GetSubmenus(){return subs;}static void SetShowContentWindow(bool){}static void SetActiveSubmenu(std::shared_ptr<Submenu>){}static ImFont* GetOptionsFont(){return nullptr;}};
}
