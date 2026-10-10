#pragma once
#include "Mock.hpp"
namespace YimMenu { class Category; }
namespace YimMenu::ClickUI { void DrawCategory(YimMenu::Category&); }
namespace YimMenu {
class UIItem { public: virtual ~UIItem()=default;virtual bool CanDraw(){return true;}virtual void Draw()=0; };
class Group:public UIItem {public:std::string name="Panel";const std::string& GetName() const {return name;}void Draw() override {++ImGui::groupDraws;} };
class Custom:public UIItem {public:void Draw() override {++ImGui::customDraws;} };
class Category {public:std::string m_Name="General";std::vector<std::shared_ptr<UIItem>> items;const auto& GetItems(){return items;}void Draw(){++ImGui::categoryDraws;ClickUI::DrawCategory(*this);} };
class Submenu {public:std::string m_Name="Self",m_Icon="";std::vector<std::shared_ptr<Category>> m_Categories;std::shared_ptr<Category> active;auto GetActiveCategory(){return active;}void SetActiveCategory(std::shared_ptr<Category> c){active=c;} };
class UIManager {public:static inline std::vector<std::shared_ptr<Submenu>> subs;static inline std::shared_ptr<Submenu> active;static const auto& GetSubmenus(){return subs;}static auto GetActiveSubmenu(){return active;}static void SetActiveSubmenu(std::shared_ptr<Submenu> s){active=s;}static void SetShowContentWindow(bool){}static ImFont* GetOptionsFont(){return nullptr;} };
}
namespace YimMenu::Menu::Font { inline ImFont* g_AwesomeFont=nullptr; }
namespace YimMenu::Submenus { inline void DrawClickPlayerDock(){++ImGui::playerDocks;}inline void DrawClickPlayerInfo(){++ImGui::playerInfo;} }
