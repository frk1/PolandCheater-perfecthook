#pragma once
#include "HookIncludes.h"
using do_post_screen_space_effects_t = bool(__thiscall*)(void*, CViewSetup*);
do_post_screen_space_effects_t odo_post_screen_space_effects;
CGlowObjectManager*   g_GlowObjManager = nullptr;

bool _fastcall do_post_screen_space_effects(void* ecx, void* edx, CViewSetup* pSetup)
{

    g_GlowObjManager = *(CGlowObjectManager**)(U::pattern_scan(GetModuleHandleW(L"client.dll"), "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00") + 3);


    if (menu.Visuals.Glow && g_GlowObjManager && I::Engine->IsConnected())
    {
        auto local = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());
        if (local)
        {
            for (int i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); ++i)
            {
                if (g_GlowObjManager->m_GlowObjectDefinitions[i].IsUnused() || !g_GlowObjManager->m_GlowObjectDefinitions[i].getEnt())
                    continue;

                CGlowObjectManager::GlowObjectDefinition_t* glowEnt = &g_GlowObjManager->m_GlowObjectDefinitions[i];

                switch (glowEnt->getEnt()->GetClientClass()->m_ClassID)
                {
                default:
                        if (strstr(glowEnt->getEnt()->GetClientClass()->m_pNetworkName, ("CWeapon")))
                            glowEnt->set(1.0f, 1.0f, 0.0f, 0.6f);
                    break;
                case 1:
                        glowEnt->set(1.0f, 1.0f, 0.0f, 0.6f);
                    break;
                case 35:

                        if (menu.Visuals.Filter.EnemyOnly && glowEnt->getEnt()->GetTeamNum() == local->GetTeamNum())
                            break;

                        if (glowEnt->getEnt()->GetTeamNum() == local->GetTeamNum())
                            glowEnt->set(0.23921f, 0.55294f, 0.89019f, 0.8f);
                        else if (glowEnt->getEnt()->GetTeamNum() != local->GetTeamNum())
                            glowEnt->set(0.89019f, 0.23137f, 0.23137f, 0.8f);
                    break;
                case 39:
                        glowEnt->set(1.0f, 1.0f, 0.0f, 0.6f);
                    break;
                }
            }
        }
    }
    return odo_post_screen_space_effects(ecx, pSetup);
}