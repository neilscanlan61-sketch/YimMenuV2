#include "core/frontend/manager/styles/ModernList/ModernList.hpp"
#include "Items.hpp"
#include "core/backend/FiberPool.hpp"

namespace YimMenu
{
	Button::Button(const std::string_view& name, std::function<void()> onClick, const ImVec2 size, const std::string_view& information) :
	    m_Name(name),
	    m_Information(information),
	    m_OnClick(onClick),
	    m_Size(size)
	{
	}

	void Button::Draw()
	{
		const bool pressed = ListUI::Active()
            ? ListUI::Row(this, m_Name.data(), "RUN")
            : ImGui::Button(m_Name.data(), m_Size);
        if (pressed)
		{
			if (m_OnClick != nullptr)
			{
				auto callback = m_OnClick;
				FiberPool::Push([callback] { callback(); });
			}
		}
	}
}