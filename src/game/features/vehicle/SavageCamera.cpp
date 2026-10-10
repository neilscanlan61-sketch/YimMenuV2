#include "core/commands/LoopedCommand.hpp"
#include "game/backend/Self.hpp"
#include "game/frontend/GUI.hpp"
#include "game/gta/Natives.hpp"
#include "types/pad/ControllerInputs.hpp"
#include "SavageCameraPolicy.hpp"

namespace YimMenu::Features
{
    class SavageCamera : public LoopedCommand
    {
        CameraPolicy::SavageEntryGuard m_Guard;
        int m_LastThirdPerson=1;
        void OnTick() override
        {
            auto vehicle=Self::GetVehicle();
            const int handle=vehicle ? vehicle.GetHandle() : 0;
            const bool savage=vehicle && vehicle.GetModel()=="savage"_J;
            const int mode=CAMERA::GET_FOLLOW_VEHICLE_CAM_VIEW_MODE();
            if (mode>=0 && mode<=2) m_LastThirdPerson=mode;
            const bool userCycles=!GUI::IsOpen() && PAD::IS_CONTROL_JUST_PRESSED(0,static_cast<int>(ControllerInputs::INPUT_NEXT_CAMERA));
            if (m_Guard.Update(handle,savage,CAMERA::IS_FOLLOW_VEHICLE_CAM_ACTIVE(),mode==4,userCycles,
                    static_cast<std::uint32_t>(MISC::GET_GAME_TIMER())))
            {
                const int context=CAMERA::GET_CAM_ACTIVE_VIEW_MODE_CONTEXT();
                if (context>=0) CAMERA::SET_CAM_VIEW_MODE_FOR_CONTEXT(context,m_LastThirdPerson);
                CAMERA::SET_FOLLOW_VEHICLE_CAM_VIEW_MODE(m_LastThirdPerson);
            }
        }
        void OnDisable() override { m_Guard.Reset(); }
    public:
        SavageCamera() : LoopedCommand("savagecameraguard","Savage Entry Camera Guard",
            "Switches an entry-time first-person Savage camera to third person once. Manual camera cycling cancels the guard; disable it to retain first-person entry.")
        { m_State=true; }
    };
    static SavageCamera _SavageCamera;
}
