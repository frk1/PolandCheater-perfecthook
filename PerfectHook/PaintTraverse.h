#pragma once
#include "HookIncludes.h"
#include "Interfaces.h"
#include <ctime>
#include "MovementRecorder.h"
typedef void(__thiscall* paint_traverse_t)(PVOID, unsigned int, bool, bool);


bool once = true;
bool once1 = false;
int width1 = 0;
int height1 = 0;
void __fastcall hkPaintTraverse(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
    static auto ofunc = hooks::panel.get_original<paint_traverse_t>(41);
	IClientEntity* pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	if (pLocal != nullptr && pLocal->IsAlive() && menu.Visuals.noscopeborder && !strcmp("HudZoom", g_Panel->GetName(vguiPanel)))
	{
		return;
	}
    ofunc(pPanels, vguiPanel, forceRepaint, allowForce);
	static unsigned int FocusOverlayPanel = 0;
	static bool FoundPanel = false;
	if (!FoundPanel)
	{
		PCHAR szPanelName = (PCHAR)g_Panel->GetName(vguiPanel);
		if (strstr(szPanelName, "FocusOverlayPanel"))
		{
			FocusOverlayPanel = vguiPanel;
			FoundPanel = true;
		}
	}
	else if (FocusOverlayPanel == vguiPanel)
	{
		if (g_Engine->IsConnected() && g_Engine->IsInGame())
		{
            static auto linegoesthrusmoke = U::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx");
            static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);
            if (menu.Visuals.NoSmoke) *(int*)(smokecout) = 0;
			Hacks::DrawHacks();


			auto m_flFlashDuration = NetVarManager->GetOffset("DT_CSPlayer", "m_flFlashDuration");
			auto m_flFlashMaxAlpha = NetVarManager->GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
			if (pLocal != nullptr)
			{
				CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
				if (menu.Visuals.NoFlash) 
				{
					*MakePtr(float*, pLocal, m_flFlashDuration) = 0.3f;
					*MakePtr(float*, pLocal, m_flFlashMaxAlpha) = 0.4f;
				}
				else
				{
					*MakePtr(float*, pLocal, m_flFlashDuration) = 5100.f;
					*MakePtr(float*, pLocal, m_flFlashMaxAlpha) = 255.f;
				}
				if (pLocal != nullptr && pLocal->IsScoped() && menu.Visuals.noscopeborder && MiscFunctions::IsSniper(pWeapon))
				{
					int width = 0;
					int height = 0;
					g_Engine->GetScreenSize(width, height);

					int centerX = static_cast<int>(width * 0.5f);
					int centerY = static_cast<int>(height * 0.5f);
					Render::Line(0, centerY, width, centerY, Color(0, 0, 0, 255));
					Render::Line(centerX, 0, centerX, height, Color(0, 0, 0, 255));
				}
			}
		}
		if (!g_Engine->IsInGame()) {
			menu.Misc.NameChangerFix = false; 
			menu.Misc.NameSpammer = 0;
			menu.Misc.NoName = false;
			menu.Misc.ChatSpamMode = false;
		}
		if (menu.Ragebot.b1g && !once)
		{
			menu.Legitbot.b1g = false;
			once = !once;
		}
		if (menu.Legitbot.b1g && once)
		{
			menu.Ragebot.b1g = false;
			once = !once;
		}

		/*if (!once1)
		{


			printf("csPlayerResource offset: %p", g_PlayerResource - reinterpret_cast<DWORD>(GetModuleHandleA("client.dll")));
			once1 = true;
		}*/
		if (menu.Visuals.Time)
		{
			std::time_t result = std::time(nullptr);
			Render::Text(1, 1, Color(255, 255, 255, 255), Render::Fonts::Time, std::asctime(std::localtime(&result)));
		}


	}
}
