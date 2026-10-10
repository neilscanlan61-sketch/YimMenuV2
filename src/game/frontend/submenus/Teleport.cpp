#include "Teleport.hpp"
#include "core/frontend/Notifications.hpp"
#include "core/backend/FiberPool.hpp"
#include "game/backend/SavedLocations.hpp"
#include "game/backend/LocationBrowserPolicy.hpp"
#include "game/backend/Self.hpp"
#include "game/frontend/items/Items.hpp"
#include <algorithm>
#include <optional>

namespace YimMenu::Submenus
{
    static void QueueSavedTeleport(SavedLocation location)
    {
        // Copy the location into the fiber: never retain a pointer into a list/map.
        FiberPool::Push([location] {
            if (!LocationBrowser::Finite(location) || !Self::GetPed())
            {
                Notifications::Show("Teleport", "The location or player is not valid.", NotificationType::Warning);
                return;
            }
            Self::GetPed().TeleportTo({location.x,location.y,location.z});
        });
    }

    void RenderCustomTeleport()
    {
        static std::string category="Default", search, newCategory="Default", newName;
        static std::optional<SavedLocation> selected;
        static std::optional<SavedLocation> pendingDelete;
        static std::string deleteCategory;
        // Both the browser and the vector-coordinate picker consume locked copies.
        const auto snapshot=SavedLocations::Snapshot();
        if (!snapshot.empty() && snapshot.find(category)==snapshot.end())
        {
            category=snapshot.begin()->first; selected.reset();
        }
        if (selected)
        {
            const auto found=snapshot.find(category);
            if (found==snapshot.end() || std::none_of(found->second.begin(),found->second.end(),
                [&](const auto& location){return location.name==selected->name;})) selected.reset();
        }
        ImGui::TextWrapped("Saved places: select a location, then Teleport. Double-click also teleports. Delete always asks for confirmation.");
        ImGui::TextUnformatted("Category");
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##saved-category",category.c_str()))
        {
            for (const auto& [name,locations]:snapshot)
                if (ImGui::Selectable(name.c_str(),name==category)) { category=name; selected.reset(); }
            ImGui::EndCombo();
        }
        ImGui::SetNextItemWidth(-1);
        InputTextWithHint("##saved-search","Search this category",&search).Draw();
        if (ImGui::Button("Save current location..."))
        {
            newCategory=category; ImGui::OpenPopup("Save current location");
        }
        const float listHeight=std::clamp(ImGui::GetContentRegionAvail().y-90.0f,80.0f,180.0f);
        if (ImGui::BeginListBox("##saved-places",ImVec2(-1,listHeight)))
        {
            auto found=snapshot.find(category);
            if (found==snapshot.end()) ImGui::TextDisabled("No saved locations. Save your current position to start.");
            else
            {
                const auto visible=LocationBrowser::Filter(found->second,search);
                if (visible.empty()) ImGui::TextDisabled("No locations match this search.");
                int index=0;
                for (const auto& location:visible)
                {
                    ImGui::PushID(index++);
                    const bool active=selected && selected->name==location.name;
                    if (active) ImGui::PushStyleColor(ImGuiCol_Button,ImGui::GetStyle().Colors[ImGuiCol_Header]);
                    const bool pressed=ImGui::Button((location.name+"###saved-place").c_str(),ImVec2(-1,0));
                    if (active) ImGui::PopStyleColor();
                    if (pressed) selected=location;
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip("%s\nX %.1f  Y %.1f  Z %.1f",location.name.c_str(),location.x,location.y,location.z);
                        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                        {
                            selected=location; QueueSavedTeleport(location);
                        }
                    }
                    ImGui::PopID();
                }
            }
            ImGui::EndListBox();
        }
        if (selected)
        {
            ImGui::TextWrapped("Selected: %s",selected->name.c_str());
            const float width=std::max(1.0f,(ImGui::GetContentRegionAvail().x-ImGui::GetStyle().ItemSpacing.x)*0.5f);
            if (ImGui::Button("Teleport",ImVec2(width,0))) QueueSavedTeleport(*selected);
            ImGui::SameLine();
            if (ImGui::Button("Delete...",ImVec2(width,0)))
            {
                pendingDelete=*selected; deleteCategory=category; ImGui::OpenPopup("Delete saved location");
            }
        }
        if (ImGui::BeginPopupModal("Save current location",nullptr,ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::SetNextItemWidth(230);
            InputTextWithHint("Category","Category name",&newCategory).Draw();
            ImGui::SetNextItemWidth(230);
            InputTextWithHint("Name","Location name",&newName).Draw();
            if (ImGui::Button("Save"))
            {
                const auto capturedCategory=LocationBrowser::Trim(newCategory);
                const auto capturedName=LocationBrowser::Trim(newName);
                if (!LocationBrowser::ValidName(capturedCategory) || !LocationBrowser::ValidName(capturedName))
                    Notifications::Show("Teleport", "Use a category and name of 1-64 bytes, without control characters or #.", NotificationType::Warning);
                else
                {
                    FiberPool::Push([capturedCategory,capturedName] {
                        if (!Self::GetPed()) return;
                        Entity entity=Self::GetPed();
                        if (auto vehicle=Self::GetVehicle()) entity=vehicle;
                        const auto coords=entity.GetPosition();
                        SavedLocation location{};
                        location.name=capturedName; location.x=coords.x; location.y=coords.y; location.z=coords.z;
                        location.yaw=entity.GetHeading();
                        if (!LocationBrowser::Finite(location) || !SavedLocations::TrySaveUnique(capturedCategory,location))
                            Notifications::Show("Teleport", "Could not save: name already exists, coordinates are invalid, or the file is not writable.",NotificationType::Warning);
                        else Notifications::Show("Teleport","Location saved.");
                    });
                    category=capturedCategory; newName.clear(); ImGui::CloseCurrentPopup();
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
        if (ImGui::BeginPopupModal("Delete saved location",nullptr,ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (pendingDelete) ImGui::Text("Delete %s from %s?",pendingDelete->name.c_str(),deleteCategory.c_str());
            if (ImGui::Button("Delete location"))
            {
                if (pendingDelete)
                {
                    const auto capturedName=pendingDelete->name;
                    const auto capturedCategory=deleteCategory;
                    FiberPool::Push([capturedCategory,capturedName] {
                        if (!SavedLocations::DeleteSavedLocation(capturedCategory,capturedName))
                            Notifications::Show("Teleport","Could not persist deletion.",NotificationType::Warning);
                    });
                    selected.reset(); pendingDelete.reset();
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Keep location")) { pendingDelete.reset(); ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
    }

    Teleport::Teleport() : Submenu::Submenu("Teleport","\xef\x82\xac")
    {
        auto quick=std::make_shared<Category>("Quick Travel");
        auto destinations=std::make_shared<Group>("Destinations");
        destinations->AddItem(std::make_shared<ConditionalItem>("autotptowaypoint"_J,std::make_shared<CommandItem>("tptowaypoint"_J),true));
        destinations->AddItem(std::make_shared<CommandItem>("tptoobjective"_J));
        auto automatic=std::make_shared<Group>("Waypoint");
        automatic->AddItem(std::make_shared<BoolCommandItem>("autotptowaypoint"_J));
        quick->AddItem(destinations); quick->AddItem(automatic);
        auto directional=std::make_shared<Category>("Directional");
        auto distance=std::make_shared<Group>("Distance");
        distance->AddItem(std::make_shared<FloatCommandItem>("directionaltpdistance"_J));
        auto forward=std::make_shared<Group>("Forward / Back");
        forward->AddItem(std::make_shared<CommandItem>("directionaltpforward"_J));
        forward->AddItem(std::make_shared<CommandItem>("directionaltpbackward"_J));
        auto sideways=std::make_shared<Group>("Left / Right");
        sideways->AddItem(std::make_shared<CommandItem>("directionaltpleft"_J));
        sideways->AddItem(std::make_shared<CommandItem>("directionaltpright"_J));
        auto vertical=std::make_shared<Group>("Up / Down");
        vertical->AddItem(std::make_shared<CommandItem>("directionaltpup"_J));
        vertical->AddItem(std::make_shared<CommandItem>("directionaltpdown"_J));
        directional->AddItem(distance); directional->AddItem(forward); directional->AddItem(sideways); directional->AddItem(vertical);
        auto saved=std::make_shared<Category>("Saved");
        saved->AddItem(std::make_shared<ImGuiItem>([] { RenderCustomTeleport(); }));
        AddCategory(std::move(quick)); AddCategory(std::move(directional)); AddCategory(std::move(saved));
    }
}
