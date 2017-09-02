

#include "Interfaces.h"
#include "Utilities.h"
//SDK Specific Definitions
typedef void* (__cdecl* CreateInterface_t)(const char*, int*);
typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

//Some globals for later
CreateInterface_t EnginePointer = NULL; // These are used to store the individual
CreateInterface_t ClientPointer = NULL; //  CreateInterface functions for each game
CreateInterface_t VGUISurfacePointer = NULL; //  dll that we need access to. Can call
CreateInterface_t VGUI2Pointer = NULL; //  them to recieve pointers to game classes.
CreateInterface_t MaterialPointer = NULL;
CreateInterface_t PhysicsPointer = NULL;
CreateInterface_t StdPointer = NULL;
CreateInterface_t CachePointer = NULL;
//CreateInterface_t scaleformui1 = NULL;
//CreateInterface_t InputSystemPointer = NULL;
C_CSPlayerResource* csPlayerResource = NULL;
C_CSGameRules** csGameRules = NULL;
IMDLCache* g_pMdlCache = NULL;
template<class T>
static T* FindHudElement(const char* name)
{
    static auto pThis = *reinterpret_cast<DWORD**>(U::pattern_scan(GetModuleHandleW(L"client.dll"), "B9 ? ? ? ? E8 ? ? ? ? 83 7D F8 ?") + 1);

    static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(U::pattern_scan(GetModuleHandleW(L"client.dll"), "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
    return (T*)find_hud_element(pThis, name);
}

void I::Initialise()
{
	//Get function pointers to the CreateInterface function of each module
	EnginePointer = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Engine, "CreateInterface");
	ClientPointer = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Client, "CreateInterface");
	VGUI2Pointer = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUI2, "CreateInterface");
	VGUISurfacePointer = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUISurface, "CreateInterface");
	MaterialPointer = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Material, "CreateInterface");
	PhysicsPointer = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VPhysics, "CreateInterface");
	StdPointer = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Stdlib, "CreateInterface");
	CachePointer = (CreateInterface_t)GetProcAddress((HMODULE)GetModuleHandle("datacache.dll"), "CreateInterface");
	//scaleformui1 = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::scaleformui, "CreateInterface");
//	InputSystemPointer = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::InputSystem, "CreateInterface");



	Client = (IBaseClientDLL*)ClientPointer("VClient018", NULL);
	Engine = (IVEngineClient*)EnginePointer("VEngineClient014", NULL);
	Panels = (IPanel*)VGUI2Pointer("VGUI_Panel009", NULL);
	Surface = (ISurface*)VGUISurfacePointer("VGUI_Surface031", NULL);
	EntityList = (IClientEntityList*)ClientPointer("VClientEntityList003", NULL);
	DebugOverlay = (IVDebugOverlay*)EnginePointer("VDebugOverlay004", NULL);
	Prediction = (IPrediction*)ClientPointer("VClientPrediction001", NULL);

	MaterialSystem = (CMaterialSystem*)MaterialPointer("VMaterialSystem080", NULL);
	RenderView = (CVRenderView*)EnginePointer("VEngineRenderView014", NULL);
	ModelRender = (IVModelRender*)EnginePointer("VEngineModel016", NULL);
	ModelInfo = (CModelInfo*)EnginePointer("VModelInfoClient004", NULL);

	Trace = (IEngineTrace*)EnginePointer("EngineTraceClient004", NULL);
	PhysProps = (IPhysicsSurfaceProps*)PhysicsPointer("VPhysicsSurfaceProps001", NULL);
	CVar = (ICVar*)StdPointer("VEngineCvar007", NULL);
	Dlight = (IVEffects*)EnginePointer("VEngineEffects001", NULL);
	GameMovement = (IGameMovement*)ClientPointer("GameMovement001", NULL);
	//ScaleformUI = (DWORD*)scaleformui1("ScaleformUI002", NULL);
	MoveHelper = **(IMoveHelper***)(U::FindPattern("client.dll", (PBYTE)"\x8B\x0D\x00\x00\x00\x00\x8B\x46\x08\x68", "xx????xxxx") + 2);
	//InputSystem = (IInputSystem*)InputSystemPointer("InputSystemVersion001", NULL);
	EventManager = (IGameEventManager2*)EnginePointer("GAMEEVENTSMANAGER002", NULL);
	GameConsole = (IGameConsole*)ClientPointer("GameConsole004", NULL);
	Input = *reinterpret_cast< CInput** >((*reinterpret_cast<DWORD**>(Client))[15] + 0x1);
	ViewRender = *(IViewRender**)(U::FindPattern("client.dll", (PBYTE)"\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xFF\x10", "x????x????xx????????xx") + 1);
	//csPlayerResource = (C_CSPlayerResource**)(0x2F12D2C + reinterpret_cast<DWORD>(GetModuleHandleA("client.dll")));
	csPlayerResource = **(C_CSPlayerResource***)(U::FindPattern("client.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7", "xx????xxxx????xx") + 2);
//	csGameRules = **(C_CSGameRules***)(U::FindPattern("client.dll", (PBYTE)"\xA1\x00\x00\x00\x00\x85\xC0\x0F\x84\x00\x00\x00\x00\x80\xB8\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\xF3\x0F\x6F\x05", "x????xxxx????xx????xx????xxxx") + 1);
	csGameRules = **(C_CSGameRules***)(U::FindPattern("client.dll", (PBYTE)"\xA1\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x6A\x00\x68\x00\x00\x00\x00\xC6\x80", "x????xx????xxx????xx") + 1);
	g_pMdlCache = (IMDLCache*)CachePointer("MDLCache004", NULL);
	//clientstate = **(CClientState***)(U::FindPattern("engine.dll", (PBYTE)"\xA1\x00\x00\x00\x00\x83\xB8\x00\x00\x00\x00\x00\x75\x42\x80\x3D", "x????xx?????xxxx") + 1);
    ChatElement = FindHudElement<CHudChat>("CHudChat");
    ClientMode = **(DWORD***)((*(DWORD**)Client)[10] + 0x5); //8B 0D ? ? ? ? FF 75 08 8B 01 FF 50 64 +2 after update


    Globals = **(CGlobalVarsBase***)((*(DWORD**)Client)[0] + 0x1B); // A3 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 6A 00 +1 after update
//	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "0x%x\n", ScaleformUI);
	//	char* gameconsolepointer = (char*)U::FindTextPattern("client.dll", "GameConsole0");
	//Get the interface names regardless of their version number by scanning for each string
	/*char* CHLClientInterfaceName = (char*)U::FindTextPattern("client.dll", "VClient0");
	char* VGUI2PanelsInterfaceName = (char*)U::FindTextPattern("vgui2.dll", "VGUI_Panel0");
	char* VGUISurfaceInterfaceName = (char*)U::FindTextPattern("vguimatsurface.dll", "VGUI_Surface0");
	char* EntityListInterfaceName = (char*)U::FindTextPattern("client.dll", "VClientEntityList0");
	char* EngineDebugThingInterface = (char*)U::FindTextPattern("engine.dll", "VDebugOverlay0");
	char* EngineClientInterfaceName = (char*)U::FindTextPattern("engine.dll", "VEngineClient0");
	char* ClientPredictionInterface = (char*)U::FindTextPattern("client.dll", "VClientPrediction0");

	char* MatSystemInterfaceName = (char*)U::FindTextPattern("materialsystem.dll", "VMaterialSystem0");
	char* EngineRenderViewInterface = (char*)U::FindTextPattern("engine.dll", "VEngineRenderView0");
	char* EngineModelRenderInterface = (char*)U::FindTextPattern("engine.dll", "VEngineModel0");
	char* EngineModelInfoInterface = (char*)U::FindTextPattern("engine.dll", "VModelInfoClient0");

	char* EngineTraceInterfaceName = (char*)U::FindTextPattern("engine.dll", "EngineTraceClient0");
	char* PhysPropsInterfaces = (char*)U::FindTextPattern("client.dll", "VPhysicsSurfaceProps0");
	char* VEngineEffect = (char*)U::FindTextPattern("engine.dll", "VEngineEffects0");
	char* VEngineCvarName = (char*)U::FindTextPattern("engine.dll", "VEngineCvar0");
	char* GameMovementName = (char*)U::FindTextPattern("client.dll", "GameMovement0");
	char* InputName = (char*)U::FindTextPattern("inputsystem.dll", "InputSystemVersion0");
	char* EventManagerPointer = (char*)U::FindTextPattern("engine.dll", "GAMEEVENTSMANAGER0");*/
	//char* gameconsolepointer = (char*)U::FindTextPattern("gameui.dll", "GameConsole0");
	/*I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", CHLClientInterfaceName);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", VGUI2PanelsInterfaceName);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", VGUISurfaceInterfaceName);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", EntityListInterfaceName);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", EngineDebugThingInterface);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", EngineClientInterfaceName);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", ClientPredictionInterface);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", MatSystemInterfaceName);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", EngineRenderViewInterface);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", EngineModelRenderInterface);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", EngineModelInfoInterface);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", EngineTraceInterfaceName);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", PhysPropsInterfaces);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", VEngineEffect);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", VEngineCvarName);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", GameMovementName);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", InputName);
	I::CVar->ConsoleColorPrintf(Color(0, 255, 0), "%s\n", EventManagerPointer);*/

//	U::Log("Interfaces found");
}



// Namespace to contain all the valve interfaces
namespace I
{
	IBaseClientDLL* Client;
	IVEngineClient* Engine;
	IPanel* Panels;
	IClientEntityList* EntityList;
	ISurface* Surface;
	IVDebugOverlay* DebugOverlay;
	DWORD* ClientMode;
	CGlobalVarsBase* Globals;
	IPrediction *Prediction;
	CMaterialSystem* MaterialSystem;
	CVRenderView* RenderView;
	IVModelRender* ModelRender;
	CModelInfo* ModelInfo;
	IEngineTrace* Trace;
	IPhysicsSurfaceProps* PhysProps;
	ICVar* CVar;
	IVEffects* Dlight;
	IMoveHelper* MoveHelper;
	IGameMovement* GameMovement;
	CInput* Input;
	IGameEventManager2 *EventManager;
	C_CSPlayerResource* csPlayerResource;
	C_CSGameRules* csGameRules;
	IViewRender* ViewRender;
	IGameConsole* GameConsole;
	IMDLCache* g_pMdlCache;
	//CClientState* clientstate;
//	DWORD* ScaleformUI;
	//	IInputSystem* InputSystem;
    CHudChat* ChatElement;
};