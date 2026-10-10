#include "Mock.hpp"
#include "../../src/core/frontend/manager/styles/ModernList/ListTheme.cpp"
#include "../../src/core/frontend/manager/styles/ModernList/ListThemeEditor.cpp"
#include "../../src/core/frontend/manager/styles/ModernList/ModernList.cpp"
#include "../../src/game/frontend/GUIInputPolicy.hpp"
int main() {
 using namespace YimMenu;
 auto sub=std::make_shared<Submenu>();
 auto cat=std::make_shared<Category>();
 sub->m_Categories.push_back(cat); UIManager::subs.push_back(sub);
 const auto originalRounding=ImGui::style.WindowRounding;
 RenderModernListTheme(); assert(!ListUI::Active());
 ImGui::focused=sub.get(); ImGui::nextKey=ImGuiKey_RightArrow;
 RenderModernListTheme(); ImGui::focused=nullptr;
 ImGui::click=cat.get(); RenderModernListTheme();
 RenderModernListTheme(); assert(ImGui::categoryDraws==1);
 ImGui::io.WantTextInput=true; ImGui::key=true;
 RenderModernListTheme(); assert(ImGui::categoryDraws==2);
 ImGui::io.WantTextInput=false; ImGui::key=false;
 ImGui::active=true; ImGui::key=true;
 RenderModernListTheme(); assert(ImGui::categoryDraws==3);
 ImGui::active=false; ImGui::key=false;
 ImGui::popup=true; ImGui::key=true;
 RenderModernListTheme(); assert(ImGui::categoryDraws==4);
 ImGui::popup=false; ImGui::key=false;
 ImGui::key=true; RenderModernListTheme(); assert(ImGui::categoryDraws==4);
 ImGui::nextKey=ImGuiKey_LeftArrow; RenderModernListTheme();
 ImGui::focused=sub.get(); ImGui::nextKey=ImGuiKey_RightArrow;
 RenderModernListTheme(); ImGui::focused=nullptr;
 ImGui::click=cat.get(); RenderModernListTheme();
 RenderModernListTheme(); assert(ImGui::categoryDraws==5);
 sub->m_Categories.clear(); RenderModernListTheme(); assert(ImGui::categoryDraws==5);
 UIManager::subs.clear(); RenderModernListTheme(); assert(ImGui::categoryDraws==5);
 ImGui::begin=false; RenderModernListTheme();
 assert(ImGui::windowBalance==0 && ImGui::childBalance==0);
 assert(!ListUI::Active()); assert(ImGui::style.WindowRounding==originalRounding);
 for(int o=0;o<2;++o) for(int l=0;l<2;++l) for(int b=0;b<2;++b) for(int e=0;e<2;++e) {
  auto policy=GetGUIInputPolicy(o,l,b,e);
  assert(policy.KeyboardList==bool(o&&l&&!b));
  assert(policy.MouseUI==bool(b||(o&&!policy.KeyboardList)));
  assert(policy.PreserveGameplay==bool(o&&l&&!b&&!e));
 }
 assert(ImGui::focusRequests>=5);
 int rowId=0; ListUI::s_Active=true; ImGui::focused=&rowId;
 ListUI::Row(&rowId,"A focused command","ON",true);
 ListUI::Describe("Test description");
 assert(ListUI::s_FocusedLabel=="A focused command");
 assert(ListUI::s_Description=="Test description");
 ListUI::s_Active=false;
 std::cout << "PASS: renderer routes, keyboard focus, Right/Left navigation, back guards, removed routes, balanced windows, style scope, footer description and 16 input policy combinations.\n";
}
