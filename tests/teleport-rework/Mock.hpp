#pragma once
#include "../click-ui/Mock.hpp"
#include <map>
#include <optional>
#include <utility>
#include <string>
#include <vector>
#include <functional>
using joaat_t=std::string;
inline std::string operator""_J(const char* s,size_t n){return {s,n};}
namespace rage {struct fvector3{float x=0,y=0,z=0;};}
namespace ImGui {
inline bool doubleClick=false;inline std::string popup;inline std::string hovered;
inline bool IsMouseDoubleClicked(int){bool value=doubleClick;doubleClick=false;return value;}
inline bool BeginListBox(const char*,ImVec2){return true;}inline void EndListBox(){}
template<class...T>void Text(const char*,T...){}
}
#define ImGuiWindowFlags_AlwaysAutoResize 32
namespace YimMenu {
inline std::map<std::string,std::string> inputs;inline std::vector<std::string> notices;inline std::vector<std::function<void()>> jobs;
inline int teleportCount=0;inline rage::fvector3 lastTeleport;inline bool pedValid=true,vehicleValid=false;
struct Entity {bool valid=true;rage::fvector3 position{1,2,3};explicit operator bool()const{return valid;}rage::fvector3 GetPosition()const{return position;}float GetHeading()const{return 45;}void TeleportTo(rage::fvector3 where){lastTeleport=where;++teleportCount;}};
namespace Self {inline Entity GetPed(){return {pedValid,{1,2,3}};}inline Entity GetVehicle(){return {vehicleValid,{4,5,6}};}}
enum class NotificationType {Warning};struct Notifications {static void Show(const std::string&,const std::string& message,NotificationType=NotificationType::Warning){notices.push_back(message);}};
struct FiberPool {static void Push(std::function<void()> f){jobs.push_back(std::move(f));}};
struct SavedLocation {std::string name;float x=0,y=0,z=0,yaw=0,pitch=0,roll=0;};
struct SavedLocations {
 inline static std::map<std::string,std::vector<SavedLocation>> locations;
 static auto Snapshot(){return locations;}
 static bool TrySaveUnique(const std::string& category,SavedLocation value){for(const auto& [c,places]:locations)for(const auto& l:places)if(l.name==value.name)return false;locations[category].push_back(value);return true;}
 static bool DeleteSavedLocation(const std::string& category,const std::string& name){auto it=locations.find(category);if(it==locations.end())return false;auto& places=it->second;places.erase(std::remove_if(places.begin(),places.end(),[&](auto& l){return l.name==name;}),places.end());return true;}
};
struct InputTextWithHint {std::string label;std::string* value;InputTextWithHint(std::string a,std::string,std::string* b):label(a),value(b){}void Draw(){auto it=inputs.find(label);if(it!=inputs.end())*value=it->second;}};
struct UIItem {virtual~UIItem()=default;};
struct CommandItem:UIItem {std::string id;CommandItem(std::string a):id(a){}};struct BoolCommandItem:CommandItem{using CommandItem::CommandItem;};struct FloatCommandItem:CommandItem{using CommandItem::CommandItem;};
struct ConditionalItem:UIItem{ConditionalItem(std::string,std::shared_ptr<CommandItem>,bool){}};
struct ImGuiItem:UIItem{std::function<void()> callback;ImGuiItem(std::function<void()> f):callback(f){}};
struct Group:UIItem{std::string name;std::vector<std::shared_ptr<UIItem>> items;Group(std::string n):name(n){}void AddItem(std::shared_ptr<UIItem> i){items.push_back(i);}};
struct Category{std::string name;std::vector<std::shared_ptr<UIItem>> items;Category(std::string n):name(n){}void AddItem(std::shared_ptr<UIItem> i){items.push_back(i);}};
struct Submenu {std::vector<std::shared_ptr<Category>> categories;Submenu(std::string,std::string){}void AddCategory(std::shared_ptr<Category> c){categories.push_back(c);}};
}
