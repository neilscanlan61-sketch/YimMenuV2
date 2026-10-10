#include "core/frontend/manager/styles/ModernList/ModernList.hpp"
#include "Items.hpp"

namespace YimMenu
{
	TabItem::TabItem(const std::string& name) :
	    m_Name(name)
	{
	}

	void TabItem::Draw()
	{
        if (ListUI::Active())
        {
            ImGui::PushID(this);
            if (ImGui::CollapsingHeader(m_Name.c_str()))
            {
                for (const auto& item : m_Items)
                {
                    if (!item || !item->CanDraw()) continue;
                    ImGui::PushID(item.get());
                    item->Draw();
                    ImGui::PopID();
                }
            }
            ImGui::PopID();
            return;
        }
		if (ImGui::BeginTabItem(m_Name.c_str()))
		{
			for (auto& item : m_Items)
			{
				if (item->CanDraw())
					item->Draw();
			}
			ImGui::EndTabItem();
		}
	}
}
