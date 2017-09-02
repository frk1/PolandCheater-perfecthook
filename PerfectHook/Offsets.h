

#pragma once

#include "CommonIncludes.h"


// Various offsets
namespace Offsets
{
	// Sets up all the shit we need
	void Initialise();

	// Addresses of loaded game modules
	namespace Modules
	{
		extern DWORD Client;
		extern DWORD Engine;
		extern DWORD VGUI2;
		extern DWORD VGUISurface;
		extern DWORD Material;
		extern DWORD VPhysics;
		extern DWORD Stdlib;
	//	extern DWORD InputSystem;
		extern DWORD scaleformui;
		extern DWORD cache;
	};



	// Addresses of engine functions to call
	namespace Functions
	{
		extern DWORD KeyValues_KeyValues;
		extern DWORD KeyValues_LoadFromBuffer;
		extern DWORD dwCalcPlayerView;
	};


};