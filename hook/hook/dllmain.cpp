// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "MyCaptureCallStack.h"
#include "MyFree.h"
#include "MyMalloc.h"
#include "MyHeapDmp.h"
#include "MyQueryMallocSize.h"
#include "MyRealloc.h"
#include "MyHeapDmp.h"

#pragma comment(lib, "libMinHook.x64.lib")
HMODULE hDllModule = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			hDllModule = hModule;
			PrintHeapDmp();
			if (MH_Initialize() != MH_OK)
			{
				abort();
			}
			InstallQueryMallocSize();
			InstallMalloc();
			InstallRealloc();
		}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		{
			UnstallMalloc();
			UnstallQueryMallocSize();
			UnstallRealloc();
		}
		break;
	}
	return TRUE;
}

