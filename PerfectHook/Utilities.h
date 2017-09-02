/*
dankmeme
*/

#pragma once

// Includes
#include "CommonIncludes.h"
#include <time.h>

// Colors for the console
//Define extra colours
#define FOREGROUND_WHITE		    (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_YELLOW       	(FOREGROUND_RED | FOREGROUND_GREEN)
#define FOREGROUND_CYAN		        (FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_MAGENTA	        (FOREGROUND_RED | FOREGROUND_BLUE)
#define FOREGROUND_BLACK		    0

#define FOREGROUND_INTENSE_RED		(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_GREEN	(FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_BLUE		(FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_WHITE	(FOREGROUND_WHITE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_YELLOW	(FOREGROUND_YELLOW | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_CYAN		(FOREGROUND_CYAN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_MAGENTA	(FOREGROUND_MAGENTA | FOREGROUND_INTENSITY)

// U Namespace
// Purpose: Contains misc functionality for memory related functionality
namespace U
{
	// Debug console controls
	void OpenConsole(std::string Title);
	void CloseConsole();
	void Log(const char *fmt, ...);

	void EnableLogFile(std::string filename);

	void SetConsoleColor(WORD color);

	// Misc Shizz
	std::string GetTimeString();

	// Memory utils
	// Purpose: Provides memeory related functionality (pattern scanning ect)

	// Waits for a module to be available, before returning it's base address
	DWORD WaitOnModuleHandle(std::string moduleName);

	// Attempts to locate the given signature and mask in a memory range
	// Returns the address at which it has been found
	DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask);

	DWORD_PTR GetPatternOffset(std::string moduleName, PBYTE pattern, std::string mask, DWORD_PTR codeBase, DWORD_PTR codeSize);
	// Attempts to locate the given text in a memory range
	// Returns the address at which it has been found
	DWORD FindTextPattern(std::string moduleName, char* string);
    std::uint8_t* pattern_scan(void* module, const char* signature);
	DWORD FindPatternV2(std::string moduleName, std::string pattern);
	template <typename T>
	T* CaptureInterface(std::string strModule, std::string strInterface)
	{
		typedef T* (*CreateInterfaceFn)(const char* szName, int iReturn);
		CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(strModule.c_str()), "CreateInterface");

		return CreateInterface(strInterface.c_str(), 0);
	}

	bool bin_match(uint8_t *code, std::vector< uint8_t > &pattern);
	template< typename T = uintptr_t > static T first_match(uintptr_t start, std::string sig, size_t len);
	template< typename T = uintptr_t > static T first_code_match(HMODULE start, std::string sig);

	class VMT
	{
	private:
		DWORD	*CustomTable;
		bool	initComplete;
		DWORD	*OriginalTable;
		DWORD	*Instance;

		int		MethodCount(DWORD* InstancePointer);

	public:
		bool	Initialize(DWORD* InstancePointer); // Pass a &class

		DWORD	Hook(DWORD NewFunction, int Index);
		void	UnhookMethod(int Index);

		void	Unhook();
		void	RestoreCustom();
		//template <class Function>
		//const Function GetOriginalFunction(std::size_t Index);

		DWORD GetOriginalFunction(int Index);
	};
};

