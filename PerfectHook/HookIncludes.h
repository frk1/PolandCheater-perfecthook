#pragma once
#define _ONSCREENSIZECHANGED_H_
#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))
#include "SDK.h"
#include "Hooks.h"
#include "Hacks.h"
#include "Chams.h"

#include "Interfaces.h"
#include "RenderManager.h"
#include "MiscHacks.h"
#include <unordered_map>

namespace Hooks
{
	// VMT Managers
	//U::VMT Client;
	U::VMT ClientMode;
	U::VMT Panel;
	U::VMT ModelRender;
	U::VMT Prediction;
	U::VMT Surface;
	U::VMT EventManager;
	U::VMT sv_cheats;
	U::VMT d3d;
	U::VMT viewrender;
	U::VMT autismform;
	U::VMT engine;
};


