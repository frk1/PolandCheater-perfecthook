#pragma once
#include "HookIncludes.h"

typedef void(__thiscall *FnOverrideView)(void* _this, CViewSetup* setup);
FnOverrideView oFnOverrideView;



void __fastcall hkOverrideView(void* _this, void* _edx, CViewSetup* setup)
{
	IClientEntity *pLocal = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());
	if (pLocal && I::Engine->IsInGame())
	{
		if (!pLocal->IsScoped())
			setup->fov += menu.Visuals.FOVChanger;
        if (menu.Visuals.ThirdPerson && pLocal->IsAlive())
        {
            if (!I::Input->m_fCameraInThirdPerson)
            {
                I::Input->m_fCameraInThirdPerson = true;
            }
        }
        else
        {
            I::Input->m_fCameraInThirdPerson = false;
        }
	}
    grenade_prediction::instance().View(setup);
	oFnOverrideView(_this, setup);
}
