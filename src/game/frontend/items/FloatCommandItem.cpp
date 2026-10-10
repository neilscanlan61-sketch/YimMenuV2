#include "core/frontend/manager/styles/ModernList/ModernList.hpp"
#include "Items.hpp"
#include "core/commands/Command.hpp"
#include "core/commands/Commands.hpp"
#include "core/commands/FloatCommand.hpp"
#include "core/frontend/widgets/toggle/imgui_toggle.hpp"

namespace YimMenu
{
	FloatCommandItem::FloatCommandItem(joaat_t id, std::optional<std::string> label_override, bool use_slider) :
	    m_useSlider(use_slider),
	    m_Command(Commands::GetCommand<FloatCommand>(id)),
	    m_LabelOverride(label_override)
	{
	}

	void FloatCommandItem::Draw()
	{
		if (!m_Command)
		{
			ImGui::Text("Unknown!");
			return;
		}

		auto value = m_Command->GetState();
		auto label = m_LabelOverride.has_value() ? m_LabelOverride.value().c_str() : m_Command->GetLabel().c_str();
        if (ListUI::Active())
        {
            ImGui::PushID(m_Command);
            const auto visibleLabel = (label[0] == '#' && label[1] == '#') ? m_Command->GetLabel().c_str() : label;
            ListUI::BeginValue(visibleLabel);
            bool changed;
            if (!m_Command->GetMinimum().has_value() || !m_Command->GetMaximum().has_value() || !m_useSlider)
                changed = ImGui::InputFloat("##value", &value);
            else
                changed = ImGui::SliderFloat("##value", &value, m_Command->GetMinimum().value(), m_Command->GetMaximum().value());
            if (changed) m_Command->SetState(value);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s", m_Command->GetDescription().c_str());
            ImGui::PopID();
            return;
        }
		if (!m_Command->GetMinimum().has_value() || !m_Command->GetMaximum().has_value() || !m_useSlider)
		{
			ImGui::SetNextItemWidth(150);
			if (ImGui::InputFloat(label, &value))
			{
				m_Command->SetState(value);
			}
		}
		else
		{
			ImGui::SetNextItemWidth(150);
			if (ImGui::SliderFloat(label, &value, m_Command->GetMinimum().value(), m_Command->GetMaximum().value()))
			{
				m_Command->SetState(value);
			}
		}
	}
}