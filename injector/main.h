#pragma once

#include <windows.h>
#include <assert.h>
#include <tlhelp32.h>
#include <iostream>
#include <excpt.h>
#include <signal.h>
#include <shlwapi.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <ostream>
using namespace std;

inline string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    string::size_type pos = string(buffer).find_last_of("\\/");
    return string(buffer).substr(0, pos);
}
inline HMODULE GetModuleHandleExtern(char *szModuleName, DWORD dwProcessId) // GetMoguleHandle recode for external processes
{
    if (!szModuleName || !dwProcessId) { return NULL; } // invalid input
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
    if (hSnap == INVALID_HANDLE_VALUE) { return NULL; }
    MODULEENTRY32 me;
    me.dwSize = sizeof(MODULEENTRY32);
    if (Module32First(hSnap, &me)) // we go now
    {
        while (Module32Next(hSnap, &me)) // through all modules in the target process
        {
            if (!strcmp(me.szModule, szModuleName)) // is this the model we are looking for?
            {
                CloseHandle(hSnap);
                return me.hModule; // this is our module, return it.
            }
        }
    }
    CloseHandle(hSnap);
    return NULL; // counldn't find module
}