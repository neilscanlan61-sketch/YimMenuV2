#pragma once
#include <cstdint>
namespace YimMenu::CameraPolicy
{
    // A bounded entry-time workaround, not a global first-person lock.
    class SavageEntryGuard
    {
        int m_Vehicle=0;
        std::uint32_t m_EnteredAt=0;
        bool m_Armed=false;
    public:
        void Reset() { m_Vehicle=0; m_Armed=false; }
        bool Update(int vehicle,bool savage,bool followCamera,bool firstPerson,bool userCycles,
            std::uint32_t now)
        {
            if (vehicle!=m_Vehicle)
            { m_Vehicle=vehicle; m_EnteredAt=now; m_Armed=vehicle!=0 && savage; }
            if (!savage || !vehicle || userCycles || std::uint32_t(now-m_EnteredAt)>1500) m_Armed=false;
            if (m_Armed && followCamera && firstPerson)
            { m_Armed=false; return true; }
            return false;
        }
    };
}
