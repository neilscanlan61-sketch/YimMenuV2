#include "Category.hpp"
#include "core/frontend/manager/styles/ModernList/ModernList.hpp"

namespace YimMenu
{
    void Category::Draw()
    {
        for (auto& item : m_Items)
        {
            if (ListUI::Active())
            {
                if (!item || !item->CanDraw()) continue;
                ImGui::PushID(item.get());
                item->Draw();
                ImGui::PopID();
            }
            else item->Draw();
        }
    }

    int Category::GetLength()
    {
        if (m_Length.has_value()) return m_Length.value();
        m_Length = std::max(ImGui::CalcTextSize(m_Name.c_str()).x + 25.0f, 75.0f);
        return m_Length.value();
    }
}
