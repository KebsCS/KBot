#pragma once

#ifndef _KBOT_H_
#define _KBOT_H_
#include <Windows.h>
#include <string>
#include <Winternl.h>
#include <Versionhelpers.h>
#include "xor.h"

__forceinline std::string GetGUID()
{
	CHAR szMainDisc[MAX_PATH];
	if (!GetLogicalDriveStringsA(sizeof(szMainDisc) - 1, szMainDisc))
		return "";

	CHAR cGuid[1024];
	if (!GetVolumeNameForVolumeMountPointA(szMainDisc, cGuid, 1024))
		return "";

	return cGuid;
}

__forceinline DWORD GetStringHash(std::string str)
{
	DWORD dwHash = 0;
	int i = str.size();

	while (i--)
	{
		dwHash ^= (dwHash >> 6) | (dwHash << 21);
		dwHash += str[i];
	}
	return dwHash;
}

__forceinline std::string GetUserNameFunc()
{
	CHAR Buff[256];
	DWORD BuffLen = 255;
	if (!GetUserNameA(Buff, &BuffLen))
		return "";

	return std::string(Buff);
}

__forceinline std::string GetComputerNameFunc()
{
	CHAR Buff2[256];
	DWORD BuffLen2 = 255;
	if (!GetComputerNameA(Buff2, &BuffLen2))
		return "";

	return std::string(Buff2);
}

__forceinline bool CheckHWID()
{
	switch (GetStringHash(GetUserNameFunc() + GetComputerNameFunc() + GetGUID()))
	{
	case 0x36e66099:
		return 1;
	default:
		abort();
		return 0;
	}

}

void CrashDebugger()
{
	CHAR OLLY_CRASH_TEXT[] = { '%', 's', '%', 's', '%', 's', '%', 's', '%', 's', '%', 's', '%', 's', '%', 's', '%', 's', '%', 's', '%', 's', 0x0 }; //%s%s%s%s%s%s%s%s%s%s%s
	OutputDebugStringA((OLLY_CRASH_TEXT));
}

/*https://anti-debug.checkpoint.com/techniques/debug-flags.html*/

typedef NTSTATUS(NTAPI* TNtQueryInformationProcess)(
	IN HANDLE           ProcessHandle,
	IN DWORD            ProcessInformationClass,
	OUT PVOID           ProcessInformation,
	IN ULONG            ProcessInformationLength,
	OUT PULONG          ReturnLength
	);

#define NtCurrentProcess				((HANDLE)(LONG_PTR)-1)
#define NtCurrentThread					((HANDLE)(LONG_PTR)-2)

#define FLG_HEAP_ENABLE_TAIL_CHECK   0x10
#define FLG_HEAP_ENABLE_FREE_CHECK   0x20
#define FLG_HEAP_VALIDATE_PARAMETERS 0x40
#define NT_GLOBAL_FLAG_DEBUGGED (FLG_HEAP_ENABLE_TAIL_CHECK | FLG_HEAP_ENABLE_FREE_CHECK | FLG_HEAP_VALIDATE_PARAMETERS)

bool IsNtQueryInformationProcess()
{
	HMODULE hNtdll = LoadLibraryA(XorStr("ntdll.dll"));
	if (hNtdll)
	{
		auto pfnNtQueryInformationProcess = (TNtQueryInformationProcess)GetProcAddress(
			hNtdll, XorStr("NtQueryInformationProcess"));

		if (pfnNtQueryInformationProcess)
		{
			DWORD dwProcessDebugPort, dwReturned;
			NTSTATUS status = pfnNtQueryInformationProcess(
				GetCurrentProcess(),
				ProcessDebugPort,
				&dwProcessDebugPort,
				sizeof(DWORD),
				&dwReturned);

			if (NT_SUCCESS(status) && (-1 == dwProcessDebugPort))
				return 1;

			DWORD dwProcessDebugFlags;
			const DWORD ProcessDebugFlags = 0x1f;
			status = pfnNtQueryInformationProcess(
				GetCurrentProcess(),
				ProcessDebugFlags,
				&dwProcessDebugFlags,
				sizeof(DWORD),
				&dwReturned);

			if (NT_SUCCESS(status) && (0 == dwProcessDebugFlags))
				return 1;

			HANDLE hProcessDebugObject = 0;
			const DWORD ProcessDebugObjectHandle = 0x1e;
			status = pfnNtQueryInformationProcess(
				GetCurrentProcess(),
				ProcessDebugObjectHandle,
				&hProcessDebugObject,
				sizeof(HANDLE),
				&dwReturned);

			if (NT_SUCCESS(status) && (0 != hProcessDebugObject))
				return 1;
		}
	}
	return 0;
}

#ifndef _WIN64
PPEB pPeb = (PPEB)__readfsdword(0x30);
DWORD dwNtGlobalFlag = *(PDWORD)((PBYTE)pPeb + 0x68);
#else
PPEB pPeb = (PPEB)__readgsqword(0x60);
DWORD dwNtGlobalFlag = *(PDWORD)((PBYTE)pPeb + 0xBC);
#endif // _WIN64

__forceinline void AntiDebug()
{
#ifndef _DEBUG

	CrashDebugger();

	//todo which is better
	//ExitProcess(-1);
	//abort();

	if (IsDebuggerPresent())
		abort();

	BOOL bDebuggerPresent;
	if (TRUE == CheckRemoteDebuggerPresent(GetCurrentProcess(), &bDebuggerPresent) &&
		TRUE == bDebuggerPresent)
		abort();

	if (IsNtQueryInformationProcess())
		abort();

	if (pPeb->BeingDebugged) //IsDebuggerPresent
		abort();

#endif // !_DEBUG
}

__forceinline void InitAntiDebug()
{
#ifndef _DEBUG

	// OS check
	if (!IsWindowsVistaOrGreater() || IsWindowsServer())
		abort();

	CHAR __NtCreateThreadEx[] = { 'N', 't', 'C', 'r', 'e', 'a', 't', 'e', 'T', 'h', 'r', 'e', 'a', 'd', 'E', 'x', 0x0 }; // NtCreateThreadEx
	HMODULE hNtdll = LoadLibraryA(XorStr("ntdll.dll"));
	if (hNtdll)
		if (!IsWindowsVistaOrGreater() && GetProcAddress(hNtdll, __NtCreateThreadEx) /* Any not XP supported windows api*/)
			abort();

	if (GetSystemMetrics(SM_CLEANBOOT) > 0)
		abort();

#endif // !_DEBUG
}

const std::vector<std::string> vWindowClasses = {
	XorStr("OLLYDBG"),
	XorStr("WinDbgFrameClass"), // WinDbg
	XorStr("ID"),               // Immunity Debugger
	XorStr("Zeta Debugger"),
	XorStr("Rock Debugger"),
	XorStr("ObsidianGUI"),
	XorStr("Qt5QWindowIcon"), //IDA PRO
};

__forceinline void CheckWindows()
{
#ifndef _DEBUG

	for (auto& sWndClass : vWindowClasses)
	{
		if (NULL != FindWindowA(sWndClass.c_str(), NULL))
			ExitProcess(-1);
	}

#endif // !_DEBUG
}

#endif