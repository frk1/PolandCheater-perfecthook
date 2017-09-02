#pragma once
#include "SkinChanger.h"
#include "Resolver.h"
#include "HookIncludes.h"
#include "GloveChanger.h"
#include "LagComp.h"
typedef void(__stdcall *FrameStageNotifyFn)(ClientFrameStage_t);
FrameStageNotifyFn oFrameStageNotify;
void __stdcall hkFrameStageNotify(ClientFrameStage_t curStage);
void  __stdcall hkFrameStageNotify(ClientFrameStage_t curStage)
{
    if (I::Engine->IsConnected() && I::Engine->IsInGame())
    {
        if (curStage == FRAME_RENDER_START)
        {
            auto pLocal = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());
            auto dwDeadFlag = NetVarManager->GetOffset("DT_CSPlayer", "deadflag"); // int
            if (pLocal)
            {
                if (pLocal->IsAlive() && I::Input->m_fCameraInThirdPerson)
                {
                    *reinterpret_cast<Vector*>(reinterpret_cast<DWORD>(pLocal) + dwDeadFlag + 4) = qLastTickAngles;
                }
            }
        }
    }
	oFrameStageNotify(curStage);
	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
        if(menu.Ragebot.Resolver) Resolver3();
        if (menu.Skinchanger.Enabled)
        {
            GloveChanger();
            SkinChanger();
        }
	}

}
