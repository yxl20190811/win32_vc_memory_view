#include "StdAfx.h"
#include "MyRealloc.h"
#include "MyCaptureCallStack.h"
#include "MyQueryMallocSize.h"
#include "MyMalloc.h"
#include "MyFree.h"


TRealloc  CrtRealloc = NULL;
TRealloc  TrunkRealloc = NULL;

void * __cdecl MyRealloc(_Post_ptr_invalid_ void * _Memory, _In_ size_t _NewSize)
{
	void         * callers_stack[ kMaxCallers ];
    unsigned short frames;
	frames    = (CrtCaptureCallStack)( 0, kMaxCallers, callers_stack, NULL ) - 4;
	
	char* ret = (char*)TrunkMalloc(_NewSize + sizeof(TCallStack) + frames*sizeof(void*));
	size_t size = MyQueryMallocSize((char*)_Memory);
	if(size > _NewSize)
	{
		size = _NewSize;
	}
	memcpy(ret, _Memory, size);
	CrtFree(_Memory);


	TCallStack * info = (TCallStack*)(ret + _NewSize + frames*sizeof(void*));
	info->m_time = ::GetTickCount64();
	info->m_deep = frames;
	info->m_flag = CallStackFlag;
	memcpy(ret + _NewSize, &(callers_stack[1]), frames*sizeof(void*));
	return ret;
}

void InstallRealloc()
{
	CrtRealloc = (TRealloc)GetProcAddress(LoadLibrary(CrtDllName), "realloc");
	if (MH_CreateHook(
		CrtRealloc, 
		&MyRealloc, 
		reinterpret_cast<void**>(&TrunkRealloc)) != MH_OK)
	{
		abort();
	}
	if (MH_EnableHook(CrtRealloc) != MH_OK)
	{
		abort();
	}
}
void UnstallRealloc()
{
	if (MH_DisableHook(CrtRealloc) != MH_OK)
	{
		abort();
	}
}


