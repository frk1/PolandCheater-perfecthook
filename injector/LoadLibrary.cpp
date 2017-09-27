#include "LoadLibrary.h"


int loadlibrary::loadlibrarymain(const char* proccessname, const char* dllname, int argc)
{
    printf("You have chosen LoadLibrary injection method\n");
    while (!LoadProcess(proccessname))
    {
        printf("Looking for a process...\n\n");
        Sleep(1000);
    }
    unsigned int pid = GetProcessPID(proccessname);
    while (!pid)
    {
        GetProcessPID(proccessname);
    }

    printf("%s found! Injecting DLL!", proccessname);
	char dllpath[512];
	if(argc == 1) sprintf_s(dllpath, sizeof(dllpath) - 1, "%s\\/%s", ExePath().c_str(), dllname);
	else if (argc == 2) sprintf_s(dllpath, sizeof(dllpath) - 1, "%s", dllname);
    if (pid)
    {
		if (strstr(proccessname, "csgo.exe"))
		{
			while (!GetModuleHandleExtern("serverbrowser.dll", pid))
			{
				Sleep(500);
			}
		}
        CreateRemoteThreadMethod(pid, dllpath);
    }
    return 0;
}

unsigned int loadlibrary::GetProcessPID(const char* process_name) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    unsigned int count = 0;
    unsigned int pid = 0;

    if (snap == INVALID_HANDLE_VALUE) {
        throw GetLastError();
    }

    if (!WaitForSingleObject(snap, 0) == WAIT_TIMEOUT) {
        return 0;
    }

    PROCESSENTRY32 proc;
    proc.dwSize = sizeof(PROCESSENTRY32);
    BOOL ret = Process32Next(snap, &proc);

    while (ret) {
        if (!_stricmp(proc.szExeFile, process_name)) {
            count++;
            pid = proc.th32ProcessID;
        }
        ret = Process32Next(snap, &proc);
    }

    if (count > 1) {
        pid = -1;
    }

    CloseHandle(snap);

    return pid;
}
bool loadlibrary::LoadProcess(const char* procName)
{
    HANDLE hProcessId = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(pEntry);

    do
    {
        if (!strcmp(pEntry.szExeFile, procName))
        {
            dwProcessId = pEntry.th32ProcessID;
            CloseHandle(hProcessId);

            hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwProcessId);
            return (processOk = true);
        }

    } while (Process32Next(hProcessId, &pEntry));

    return (processOk = false);
}

bool loadlibrary::CreateRemoteThreadMethod(unsigned int pid, const char* dll_path) {
    HANDLE process;
    process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    LPVOID loadLibraryAddress;
    loadLibraryAddress = LPVOID(GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA"));

    LPVOID memory;
    memory = LPVOID(VirtualAllocEx(process, nullptr, strlen(dll_path) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));

    WriteProcessMemory(process, LPVOID(memory), dll_path, strlen(dll_path) + 1, nullptr);
    CreateRemoteThread(process, nullptr, NULL, LPTHREAD_START_ROUTINE(loadLibraryAddress), LPVOID(memory), NULL, nullptr);

    CloseHandle(process);
    VirtualFreeEx(process, LPVOID(memory), 0, MEM_RELEASE);

    return true;
}

loadlibrary* load_library = new(loadlibrary);