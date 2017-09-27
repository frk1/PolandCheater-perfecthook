#pragma once
#include "main.h"

typedef HMODULE(WINAPI *pLoadLibraryA)(LPCSTR);
typedef FARPROC(WINAPI *pGetProcAddress)(HMODULE, LPCSTR);

typedef BOOL(WINAPI *PDLL_MAIN)(HMODULE, DWORD, PVOID);

typedef struct _MANUAL_INJECT
{
    PVOID ImageBase;
    PIMAGE_NT_HEADERS NtHeaders;
    PIMAGE_BASE_RELOCATION BaseRelocation;
    PIMAGE_IMPORT_DESCRIPTOR ImportDirectory;
    pLoadLibraryA fnLoadLibraryA;
    pGetProcAddress fnGetProcAddress;
}MANUAL_INJECT, *PMANUAL_INJECT;


class manualmap
{
public:
    int manualmapmain(const char* proccessname, const char* dllname, int argc);
private:
    int map(unsigned int pid, LPCSTR dllname, LPCSTR exename);
    unsigned int GetProcessPID(const char* process_name);
    bool LoadProcess(const char* procName);
private:
    HANDLE hProcess = NULL;
    DWORD dwProcessId = NULL;
    bool processOk = false;
    unsigned int PID;
};

extern manualmap* manual_map;