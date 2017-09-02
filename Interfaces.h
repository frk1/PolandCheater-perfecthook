#pragma once

// Includes
#include "CommonIncludes.h"
#include "SDK.h"

// Namespace to contain all the valve interfaces
namespace I
{
	// Gets handles to all the interfaces needed
	void Initialise();


	extern IBaseClientDLL* Client;
	extern IVEngineClient* Engine;
	extern IPanel* Panels;
	extern IClientEntityList* EntityList;
	extern ISurface* Surface;
	extern IVDebugOverlay* DebugOverlay;
	extern DWORD* ClientMode;
	extern CGlobalVarsBase* Globals;
	extern IPrediction* Prediction;
	extern CMaterialSystem* MaterialSystem;
	extern CVRenderView* RenderView;
	extern IVModelRender* ModelRender;
	extern CModelInfo* ModelInfo;
	extern IEngineTrace* Trace;
	extern IPhysicsSurfaceProps* PhysProps;
	extern ICVar* CVar;
	extern IVEffects* Dlight;
	extern IMoveHelper* MoveHelper;
//	extern IInputSystem* InputSystem;
	extern IGameMovement* GameMovement;
	extern CInput* Input;
	extern IGameEventManager2 *EventManager;
	extern C_CSPlayerResource* csPlayerResource;
	extern C_CSGameRules* csGameRules;
	extern IViewRender* ViewRender;
	extern IGameConsole* GameConsole;
	extern IMDLCache* g_pMdlCache;
	//extern CClientState* clientstate;
//	extern DWORD* ScaleformUI;
    extern CHudChat* ChatElement;
};

