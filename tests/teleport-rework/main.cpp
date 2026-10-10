#include "Mock.hpp"
#include "../../src/game/backend/LocationBrowserPolicy.hpp"
#include "../../src/game/frontend/submenus/Teleport.cpp"
#include <limits>
int main(){
 using namespace YimMenu;using namespace YimMenu::LocationBrowser;
 assert(Trim("  Home \t")=="Home");assert(Trim(" \t\n").empty());assert(ValidName("Home"));assert(!ValidName(""));assert(!ValidName(std::string(65,'x')));assert(!ValidName("Bad##ID"));assert(!ValidName("Bad\n"));
 std::vector<SavedLocation> places{{"Beach",1,2,3},{"Garage",4,5,6}};assert(Filter(places,"BEA").size()==1);assert(Filter(places,"").size()==2);assert(Filter(places,"unknown").empty());assert(Finite(places[0]));auto invalid=places[0];invalid.x=std::numeric_limits<float>::infinity();assert(!Finite(invalid));
 Submenus::Teleport submenu;assert(submenu.categories.size()==3);assert(submenu.categories[0]->name=="Quick Travel"&&submenu.categories[1]->name=="Directional"&&submenu.categories[2]->name=="Saved");assert(submenu.categories[1]->items.size()==4);
 auto run=[] {auto tasks=std::move(jobs);jobs.clear();for(auto& task:tasks)task();};
 SavedLocations::locations["Default"]={{"Beach",1,2,3}};Submenus::RenderCustomTeleport();
 ImGui::click="Beach###saved-place";Submenus::RenderCustomTeleport();assert(jobs.empty());ImGui::click="Teleport";Submenus::RenderCustomTeleport();assert(jobs.size()==1);SavedLocations::locations["Default"][0].x=999;run();assert(teleportCount==1&&lastTeleport.x==1); // queued copy, not a map pointer
 ImGui::pop=true;inputs["Category"]="  Holiday  ";inputs["Name"]="  Pier  ";vehicleValid=true;ImGui::click="Save";Submenus::RenderCustomTeleport();assert(jobs.size()==1);inputs["Name"]="Changed before fiber";run();assert(SavedLocations::locations["Holiday"].front().name=="Pier");assert(SavedLocations::locations["Holiday"].front().x==4);assert(notices.back()=="Location saved.");
 assert(!SavedLocations::TrySaveUnique("Other",{"Pier",0,0,0}));
 inputs["Name"]="bad##name";ImGui::click="Save";Submenus::RenderCustomTeleport();assert(jobs.empty());
 ImGui::pop=false;inputs.clear();ImGui::click="Pier###saved-place";Submenus::RenderCustomTeleport();ImGui::click="Delete...";Submenus::RenderCustomTeleport();assert(jobs.empty());ImGui::pop=true;ImGui::click="Delete location";Submenus::RenderCustomTeleport();assert(jobs.size()==1);run();assert(SavedLocations::locations["Holiday"].empty());
 const auto count=teleportCount;Submenus::QueueSavedTeleport(invalid);run();assert(teleportCount==count);pedValid=false;Submenus::QueueSavedTeleport({"Home",1,2,3});run();assert(teleportCount==count);
 assert(ImGui::ids==0&&ImGui::styleColors==0);std::cout<<"PASS: real Teleport.cpp against mocks; category routing, name/search policies, selection, deferred-copy teleport, vehicle-position save, duplicate guard and confirmed deletion. Native game calls and disk persistence not exercised.\n";
}
