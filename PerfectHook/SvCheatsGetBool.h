#pragma once
#include "HookIncludes.h"

typedef bool(__thiscall *fnhkSvCheatsGetBool)(PVOID);
fnhkSvCheatsGetBool oSvCheatsGetBool;



auto dwCAM_Think = U::pattern_scan(GetModuleHandleW(L"client.dll"), "85 C0 75 30 38 86");

bool __fastcall hkSvCheatsGetBool(PVOID pConVar, void* edx)
{

	if (!oSvCheatsGetBool)
		return false;

	if (reinterpret_cast<DWORD>(_ReturnAddress()) == reinterpret_cast<DWORD>(dwCAM_Think))
		return true;
	return oSvCheatsGetBool(pConVar);
}