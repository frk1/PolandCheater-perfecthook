#include "HookIncludes.h"
#include "HooksList.h"
#include "Listener.h"

std::unique_ptr<VMTHook> d3d9_hook;
typedef float(__thiscall *GetViewModelFOVFn)(void*);
float __fastcall Hooked_GetViewModelFOV(void* ecx, void* edx);
bool __fastcall IsHLTV(void* ecx, void* edx);
GetViewModelFOVFn oGetViewModelFOVFn;
typedef bool(__thiscall* memeless)(void*);
memeless oUselles;
CVMTHookManager* g_pClientVMTHook = nullptr;
using is_hltv_t = bool(__thiscall*)(void*);
is_hltv_t oIsHltv;
bool __fastcall is_hltv(void* ecx, void* edx);
uint8_t* m_present;
uint8_t* m_reset;

void Hooks::Hook()
{

	//EventManager.Initialize(reinterpret_cast<DWORD*>(I::EventManager));
	//oFireEventClientSide = reinterpret_cast<FireEventClientSideFn>(EventManager.Hook(reinterpret_cast<DWORD>(&hkFireEventClientSide), 9));

	g_pClientVMTHook = new CVMTHookManager((PDWORD*)I::Client);
	oFrameStageNotify = reinterpret_cast<FrameStageNotifyFn>(g_pClientVMTHook->HookMethod(reinterpret_cast<DWORD>(hkFrameStageNotify), 36));
	originalCreateMove = reinterpret_cast<CreateMoveFn>(g_pClientVMTHook->HookMethod(reinterpret_cast<DWORD>(hkCreateMove), 21));


	Prediction.Initialize(reinterpret_cast<DWORD*>(I::Prediction));
	oInPrediction = reinterpret_cast<InPrediction_>(Prediction.Hook(reinterpret_cast<DWORD>(hkInPrediction), 14));

	ClientMode.Initialize(static_cast<DWORD*>(I::ClientMode));
	oFnOverrideView = reinterpret_cast<FnOverrideView>(ClientMode.Hook(reinterpret_cast<DWORD>(hkOverrideView), 18));
	oGetViewModelFOVFn = reinterpret_cast<GetViewModelFOVFn>(ClientMode.Hook(reinterpret_cast<DWORD>(Hooked_GetViewModelFOV), 35));
    //odo_post_screen_space_effects = reinterpret_cast<do_post_screen_space_effects_t>(ClientMode.Hook(reinterpret_cast<DWORD>(do_post_screen_space_effects), 44));

	Panel.Initialize(reinterpret_cast<DWORD*>(I::Panels));
	oPaintTraverse = reinterpret_cast<PaintTraverse_>(Panel.Hook(reinterpret_cast<DWORD>(hkPaintTraverse), 41));

	ModelRender.Initialize(reinterpret_cast<DWORD*>(I::ModelRender));
	oDrawModelExecute = reinterpret_cast<DrawModelEx_>(ModelRender.Hook(reinterpret_cast<DWORD>(hkDrawModelExecute), 21));

	Surface.Initialize(reinterpret_cast<DWORD*>(I::Surface));
	oOnScreenSizeChanged = reinterpret_cast<OnScreenSizeChangedFn>(Surface.Hook(reinterpret_cast<DWORD>(OnScreenSizeChanged), 116));
	g_fnOriginalPlaySound = reinterpret_cast<PlaySound_t>(Surface.Hook(reinterpret_cast<DWORD>(Hooked_PlaySound), 82));


    //engine.Initialize(reinterpret_cast<DWORD*>(I::Engine));
    //oIsHltv = reinterpret_cast<is_hltv_t>(engine.Hook(reinterpret_cast<DWORD>(is_hltv), 93));


    ConVar* sv_cheats_con = I::CVar->FindVar("sv_cheats");
    sv_cheats.Initialize((DWORD*)sv_cheats_con);
    oSvCheatsGetBool = (fnhkSvCheatsGetBool)sv_cheats.Hook((DWORD)hkSvCheatsGetBool, 13);



    m_present = U::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 DB 74 1F") + 0x2;//big ( large ) obs bypass
    m_reset = U::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 FF 78 18") + 0x2;  //big ( large ) obs bypass



    Present_O = **reinterpret_cast<Present_T**>(m_present);
    g_fnOriginalReset = **reinterpret_cast<Reset_t**>(m_reset);

    **reinterpret_cast<void***>(m_present) = reinterpret_cast<void*>(&Present_H);
    **reinterpret_cast<void***>(m_reset) = reinterpret_cast<void*>(&hkReset);

	//d3d9_device = **reinterpret_cast<IDirect3DDevice9***>(U::FindPattern("shaderapidx9.dll", (PBYTE)"\xA1\x00\x00\x00\x00\x50\x8B\x08\xFF\x51\x0C", "x????xxxxxx") + 1);

	//renderer->Initialize(FindWindowA("Valve001", NULL), d3d9_device);


	//d3d.Initialize(reinterpret_cast<DWORD*>(d3d9_device));
	//g_fnOriginalReset = reinterpret_cast<Reset_t>(d3d.Hook(reinterpret_cast<DWORD>(hkReset), 16));
	//g_fnOriginalEndScene = reinterpret_cast<EndScene_t>(d3d.Hook(reinterpret_cast<DWORD>(hkEndScene), 42));
    item_purchase::singleton()->initialize();
}
void Hooks::Unhook()
{
    item_purchase::singleton()->remove();
	g_pClientVMTHook->UnHook();
	Prediction.Unhook();
	ClientMode.Unhook();
	Panel.Unhook();
	ModelRender.Unhook();
	Surface.Unhook();
	//EventManager.Unhook();
	viewrender.Unhook();
	//d3d.Unhook();
    sv_cheats.Unhook();
    //engine.Unhook();
    **reinterpret_cast<void***>(m_present) = reinterpret_cast<void*>(Present_O);
    **reinterpret_cast<void***>(m_reset) = reinterpret_cast<void*>(g_fnOriginalReset);
	renderer->~Renderer();
}

float __fastcall Hooked_GetViewModelFOV(void* ecx, void* edx)
{
	float viewmodelFOV = oGetViewModelFOVFn(ecx);
	return menu.Visuals.viewmodelChanger;
}

bool __fastcall is_hltv(void* ecx, void* edx)
{
    uintptr_t pvs_entity;
    __asm
    {
        mov pvs_entity, edi
    }


    void* returna = _ReturnAddress();
    static auto v1 = (DWORD)U::pattern_scan(GetModuleHandleW(L"client.dll"), "84 C0 0F 85 ? ? ? ? A1 ? ? ? ? 8B B7");
    if (reinterpret_cast<DWORD>(returna) == (v1))
    {
        if (pvs_entity)
        {
            *(int*)(pvs_entity + 0xA24) = -1;
            *(int*)(pvs_entity + 0xA2C) = *(int *)(pvs_entity + 0xA28);
            *(int*)(pvs_entity + 0xA28) = 0;
            return true;
        }
    }
    return oIsHltv(ecx);
}