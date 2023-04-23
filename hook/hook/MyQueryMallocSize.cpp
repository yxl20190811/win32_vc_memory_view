#include "StdAfx.h"
#include "MyQueryMallocSize.h"
#include <process.h>
#include <MinHook.h>

TQueryMallocSize CrtQueryMallocSize = NULL;
TQueryMallocSize TrunkQueryMallocSize = NULL;

void InstallQueryMallocSize()
{
	CrtQueryMallocSize = (TQueryMallocSize)GetProcAddress(LoadLibrary(CrtDllName), "_msize");
	if (MH_CreateHook(
		CrtQueryMallocSize, 
		&MyQueryMallocSize, 
		reinterpret_cast<void**>(&TrunkQueryMallocSize)) != MH_OK)
	{
		abort();
	}
	if (MH_EnableHook(CrtQueryMallocSize) != MH_OK)
	{
		abort();
	}
}
void UnstallQueryMallocSize()
{
	if (MH_DisableHook(CrtQueryMallocSize) != MH_OK)
	{
		abort();
	}
}


size_t  __cdecl MyQueryMallocSize(char* _Memor)
{
	if(NULL == _Memor)
	{
		return 0;
	}
	size_t size = TrunkQueryMallocSize(_Memor);
	TCallStack* info = (TCallStack*)(_Memor + size - sizeof(TCallStack));
	if(CallStackFlag != info->m_flag)
	{
		return size;
	}
	return (size - sizeof(TCallStack) - sizeof(void*)*info->m_deep);
}

