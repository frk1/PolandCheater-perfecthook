#include "Interfaces.h"
#include "Utilities.h"

IBaseClientDLL* g_Client;
IVEngineClient* g_Engine;
IPanel* g_Panel;
IClientEntityList* g_EntityList;
ISurface* g_Surface;
IVDebugOverlay* g_DebugOverlay;
IClientMode* g_ClientMode;
CGlobalVarsBase* g_Globals;
IPrediction *g_Prediction;
CMaterialSystem* g_MaterialSystem;
CVRenderView* g_RenderView;
IVModelRender* g_ModelRender;
CModelInfo* g_ModelInfo;
IEngineTrace* g_Trace;
IPhysicsSurfaceProps* g_PhysProps;
ICVar* g_CVar;
IVEffects* g_Dlight;
IMoveHelper* g_MoveHelper;
IGameMovement* g_GameMovement;
CInput* g_Input;
IGameEventManager2* g_EventManager;
C_CSPlayerResource* g_PlayerResource;
C_CSGameRules* g_GameRules;
IViewRender* g_ViewRender;
IGameConsole* g_GameConsole;
IMDLCache* g_MdlCache;
CHudChat* g_ChatElement;
void InitialiseInterfaces()
{
    auto EnginePointer = get_module_factory(GetModuleHandleW(L"engine.dll"));
    auto ClientPointer = get_module_factory(GetModuleHandleW(L"client.dll"));
    auto VGUISurfacePointer = get_module_factory(GetModuleHandleW(L"vguimatsurface.dll")); 
    auto VGUI2Pointer = get_module_factory(GetModuleHandleW(L"vgui2.dll")); 
    auto MaterialPointer = get_module_factory(GetModuleHandleW(L"materialsystem.dll"));
    auto PhysicsPointer = get_module_factory(GetModuleHandleW(L"vphysics.dll"));
    auto StdPointer = get_module_factory(GetModuleHandleW(L"vstdlib.dll"));
    auto CachePointer = get_module_factory(GetModuleHandleW(L"datacache.dll"));

    g_Client = (IBaseClientDLL*)ClientPointer("VClient018", NULL);
    g_Engine = (IVEngineClient*)EnginePointer("VEngineClient014", NULL);
    g_Panel = (IPanel*)VGUI2Pointer("VGUI_Panel009", NULL);
	g_Surface = (ISurface*)VGUISurfacePointer("VGUI_Surface031", NULL);
	g_EntityList = (IClientEntityList*)ClientPointer("VClientEntityList003", NULL);
	g_DebugOverlay = (IVDebugOverlay*)EnginePointer("VDebugOverlay004", NULL);
	g_Prediction = (IPrediction*)ClientPointer("VClientPrediction001", NULL);
	g_MaterialSystem = (CMaterialSystem*)MaterialPointer("VMaterialSystem080", NULL);
    g_RenderView = (CVRenderView*)EnginePointer("VEngineRenderView014", NULL);
    g_ModelRender = (IVModelRender*)EnginePointer("VEngineModel016", NULL);
    g_ModelInfo = (CModelInfo*)EnginePointer("VModelInfoClient004", NULL);
    g_Trace = (IEngineTrace*)EnginePointer("EngineTraceClient004", NULL);
    g_PhysProps = (IPhysicsSurfaceProps*)PhysicsPointer("VPhysicsSurfaceProps001", NULL);
    g_CVar = (ICVar*)StdPointer("VEngineCvar007", NULL);
    g_Dlight = (IVEffects*)EnginePointer("VEngineEffects001", NULL);
    g_GameMovement = (IGameMovement*)ClientPointer("GameMovement001", NULL);
    g_MoveHelper = **(IMoveHelper***)(U::FindPattern("client.dll", (PBYTE)"\x8B\x0D\x00\x00\x00\x00\x8B\x46\x08\x68", "xx????xxxx") + 2);
    g_EventManager = (IGameEventManager2*)EnginePointer("GAMEEVENTSMANAGER002", NULL);
    g_GameConsole = (IGameConsole*)ClientPointer("GameConsole004", NULL);
    g_Input = *reinterpret_cast< CInput** >((*reinterpret_cast<DWORD**>(g_Client))[15] + 0x1);
    g_ViewRender = *(IViewRender**)(U::FindPattern("client.dll", (PBYTE)"\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xFF\x10", "x????x????xx????????xx") + 1);
    g_PlayerResource = **(C_CSPlayerResource***)(U::FindPattern("client.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7", "xx????xxxx????xx") + 2);
    g_GameRules = **(C_CSGameRules***)(U::FindPattern("client.dll", (PBYTE)"\xA1\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x6A\x00\x68\x00\x00\x00\x00\xC6\x80", "x????xx????xxx????xx") + 1);
	g_MdlCache = (IMDLCache*)CachePointer("MDLCache004", NULL);
    g_ChatElement = FindHudElement<CHudChat>("CHudChat");
    g_ClientMode = **(IClientMode***)((*(DWORD**)g_Client)[10] + 0x5);
    g_Globals = **(CGlobalVarsBase***)((*(DWORD**)g_Client)[0] + 0x1B);

}



