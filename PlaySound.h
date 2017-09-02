#pragma once
#include "HookIncludes.h"
typedef void(__thiscall* PlaySound_t)(void*, const char*);
void      __stdcall Hooked_PlaySound(const char* szFileName);
PlaySound_t                        g_fnOriginalPlaySound;
using IsReadyFn = void(__cdecl*)();
IsReadyFn IsReady;
void __stdcall Hooked_PlaySound(const char* szFileName)
{
	//Call original PlaySound
	g_fnOriginalPlaySound(I::Surface, szFileName);

	if (I::Engine->IsInGame()) return;

	if (strstr(szFileName, "UI/competitive_accept_beep.wav")) 
	{

        static auto CServerConfirmedReservationCheckCallback__IsReady
            = (void(__cdecl*)(void))U::pattern_scan(
                GetModuleHandleW(L"client.dll"),
                "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE"
            );
        CServerConfirmedReservationCheckCallback__IsReady();
	}
}
