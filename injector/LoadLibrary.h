#pragma once
#include "main.h"

class loadlibrary
{
public:
    int loadlibrarymain(const char* proccessname, const char* dllname, int argc);
private:
    unsigned int GetProcessPID(const char* process_name);
    bool LoadProcess(const char* procName);
    bool CreateRemoteThreadMethod(unsigned int pid, const char* dll_path);
private:
    HANDLE hProcess = NULL;
    DWORD dwProcessId = NULL;
    bool processOk = false;
};
extern loadlibrary* load_library;