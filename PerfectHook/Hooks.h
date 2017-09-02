#pragma once
extern bool DoUnload;
#include "Utilities.h"
#include "vmthook.h"

namespace Hooks
{
	void Hook();
	void Unhook();
	extern U::VMT Panel; // Hooking drawing functions
//	extern U::VMT Client; // Maybe CreateMove
	extern U::VMT ClientMode; // CreateMove for functionality
	extern U::VMT ModelRender; // DrawModelEx for chams
	extern U::VMT Prediction; // InPrediction for no vis recoil
	extern U::VMT Surface;
	extern U::VMT EventManager;
	extern U::VMT d3d;
	extern U::VMT viewrender;
	extern U::VMT autismform;
	extern U::VMT engine;
};
extern std::unique_ptr<VMTHook> d3d9_hook;
extern CVMTHookManager* g_pClientVMTHook;