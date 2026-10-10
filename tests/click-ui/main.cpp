#include "Mock.hpp"
#include "Frontend.hpp"
#include "../../src/core/frontend/manager/styles/CheraxClick/ClickTheme.cpp"
#include "../../src/core/frontend/manager/styles/CheraxClick/ClickThemeEditor.cpp"
#include "../../src/core/frontend/manager/styles/CheraxClick/ClickBookmarks.cpp"
#include "../../src/core/frontend/manager/styles/CheraxClick/CheraxClick.cpp"
#include "../../src/game/frontend/GUIInputPolicy.hpp"
int main(int argc,char** argv)
{
 using namespace YimMenu;using namespace YimMenu::ClickUI;
 assert(argc==2);const std::filesystem::path folder(argv[1]);InitializeThemes(folder);
 assert(ThemeNames().size()==4);assert(ActiveThemeName()=="Rockstar Compact");assert(CurrentTheme().Width==720&&CurrentTheme().Height==560&&CurrentTheme().BackgroundEnabled==1);
 CurrentTheme().Width=920;assert(SaveTheme("My Click"));assert(LoadTheme("Red Glow"));assert(LoadTheme("My Click"));assert(CurrentTheme().Width==920);
 const auto previous=CurrentTheme().Width;
 for(const char* bad:{"Width 1","Accent 1 1 1 2","PlayerWidth 999","Scanlines 0.9","Unknown 1","BackgroundEnabled 2","BackgroundOpacity -1","BackgroundSpeed 0"}) { std::ofstream(folder/"Bad.theme")<<bad;assert(!LoadTheme("Bad"));assert(CurrentTheme().Width==previous); }
 assert(!SaveTheme("../bad"));assert(!SaveTheme("CON"));
 Bookmarks bookmarks;assert(bookmarks.Load(folder/"bookmarks-test.txt"));assert(bookmarks.Toggle("Self","General"));assert(bookmarks.Contains("Self","General"));
 Bookmarks reopened;assert(reopened.Load(folder/"bookmarks-test.txt"));assert(reopened.Contains("Self","General"));assert(reopened.Toggle("Self","General"));assert(!reopened.Contains("Self","General"));
 std::ofstream(folder/"bad-bookmarks.txt")<<"\"truncated\"";assert(!reopened.Load(folder/"bad-bookmarks.txt"));
 assert(Matches("General Settings","SETTINGS"));assert(!Matches("General","Players"));assert(ColumnCount(499)==1&&ColumnCount(500)==2);
 for(int o=0;o<2;++o)for(int l=0;l<2;++l)for(int c=0;c<2;++c)for(int b=0;b<2;++b)assert(ReservePhone(o,l,c,b)==bool(o&&(l||c)&&!b));
 auto policy=GetGUIInputPolicy(true,false,false,false);assert(policy.MouseUI&&!policy.KeyboardList&&!policy.PreserveGameplay);
 auto sub=std::make_shared<Submenu>();auto cat=std::make_shared<Category>();sub->m_Categories.push_back(cat);sub->active=cat;cat->items.push_back(std::make_shared<Group>());cat->items.push_back(std::make_shared<Group>());cat->items.push_back(std::make_shared<Custom>());UIManager::subs.push_back(sub);UIManager::active=sub;
 const auto rounding=ImGui::style.WindowRounding;RenderCheraxClickTheme();assert(!Active());assert(ImGui::style.WindowRounding==rounding);assert(ImGui::groupDraws==2&&ImGui::customDraws==1);
 ImGui::availableWidth=400;RenderCheraxClickTheme();assert(ImGui::groupDraws==4);
 sub->m_Name="Players";ImGui::availableWidth=740;RenderCheraxClickTheme();assert(ImGui::playerDocks==1&&ImGui::playerInfo==1);
 ImGui::click="+ Save";RenderCheraxClickTheme();assert(s_Bookmarks.Contains("Players","General"));
 ImGui::pop=true;RenderCheraxClickTheme();ImGui::pop=false;
 ImGui::child=false;RenderCheraxClickTheme();ImGui::child=true;
 ImGui::begin=false;RenderCheraxClickTheme();ImGui::begin=true;
 UIManager::subs.clear();UIManager::active.reset();RenderCheraxClickTheme();
 assert(!Active());assert(ImGui::windows==0&&ImGui::children==0&&ImGui::ids==0&&ImGui::styleColors==0&&ImGui::fonts==0);
 s_Active=true;int draws=0;DrawPanel(&draws,"Test panel",[&]{++draws;});s_Active=false;assert(draws==1&&ImGui::groups==0&&ImGui::itemWidths==0&&ImGui::ids==0);
 std::cout<<"PASS: click renderer routes, card columns, panel callback, player docks, empty/clipped windows, popup paths, stack/style restoration, theme storage, bookmarks, filters and input policy.\n";
}
