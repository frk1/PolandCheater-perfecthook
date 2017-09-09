// General shit
#include "DLLMain.h"
#include "Utilities.h"

#include <iostream>
#include <chrono>
#include <ratio>
#include <thread>

// Stuff to initialise
#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"
#include "RenderManager.h"
#include "Hacks.h"

#include "recvproxy.h"
#include "MiscHacks.h"



bool DoUnload;



bool on_dll_attach(void* base)
{

    Offsets::Initialise();
    InitialiseInterfaces();
    NetVarManager->Initialize();


    Offsetz::GetOffsets();
    Render::Initialise();
    Hacks::SetupHacks();
    hooks::initialize();
    NetvarHook();
    AnimationFixHook();






    while (DoUnload == false)
    {
        Sleep(1000);
    }


    AnimationFixUnhook();
    UnloadProxy();
    hooks::cleanup();

    Sleep(2000);
    FreeLibraryAndExitThread((HMODULE)base, 0);
}






bool on_dll_detach()
{

    AnimationFixUnhook();
    UnloadProxy();
    hooks::cleanup();

    return 1;
}

BOOL WINAPI DllMain(
    _In_      HINSTANCE hinstDll,
    _In_      DWORD     fdwReason,
    _In_opt_	LPVOID    lpvReserved
)
{
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)on_dll_attach, hinstDll, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        if (lpvReserved == nullptr)
            return on_dll_detach();
    }
    return TRUE;
}