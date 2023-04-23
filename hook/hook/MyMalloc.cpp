#include "StdAfx.h"
#include "MyMalloc.h"
#include "MyCaptureCallStack.h"
#include <process.h>
#include <MinHook.h>

TMalloc CrtMalloc = NULL;
TMalloc  TrunkMalloc = NULL;


void * __cdecl MyMalloc(_In_ size_t _Size)
{
	void         * callers_stack[ kMaxCallers ];
    unsigned short frames = 0;
	frames    = (CrtCaptureCallStack)( 0, kMaxCallers, callers_stack, NULL ) - 4;
	char* ret = (char*)TrunkMalloc(_Size + sizeof(TCallStack) + frames*sizeof(void*));
	TCallStack * info = (TCallStack*)(ret + _Size + frames*sizeof(void*));
	info->m_time = ::GetTickCount64();
	info->m_deep = frames;
	info->m_flag = CallStackFlag;
	memcpy(ret + _Size, &(callers_stack[1]), frames*sizeof(void*));
	return ret;
}

void InstallMalloc()
{
	CrtMalloc = (TMalloc)GetProcAddress(LoadLibrary(CrtDllName), "malloc");
	if (MH_CreateHook(
		CrtMalloc, 
		&MyMalloc, 
		reinterpret_cast<void**>(&TrunkMalloc)) != MH_OK)
	{
		abort();
	}
	if (MH_EnableHook(CrtMalloc) != MH_OK)
	{
		abort();
	}
}
void UnstallMalloc()
{
	if (MH_DisableHook(CrtMalloc) != MH_OK)
	{
		abort();
	}
}





