#pragma once
#define _ONSCREENSIZECHANGED_H_
#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))
#include "SDK.h"
#include "Hooks.h"
#include "Chams.h"

#include "Interfaces.h"
#include "RenderManager.h"
#include "MiscHacks.h"
#include <unordered_map>

