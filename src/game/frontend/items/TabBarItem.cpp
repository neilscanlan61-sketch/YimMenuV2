#include "core/frontend/manager/styles/ModernList/ModernList.hpp"
#include "Items.hpp"

namespace YimMenu
{
	TabBarItem::TabBarItem(const std::string& id) :
		m_Id(id)
	{
	}

	void TabBarItem::Draw()
	{
        if (ListUI::Active())
        {
            ImGui::PushID(m_Id.c_str());
            for (const auto& tab : m_Tabs)
                if (tab && tab->CanDraw()) tab->Draw();
            ImGui::PopID();
            return;
        }
		if (ImGui::BeginTabBar(m_Id.c_str()))
		{
			for (const auto& tab : m_Tabs)
			{
				if (tab->CanDraw())
					tab->Draw();
			}
			ImGui::EndTabBar();
		}
	}
}