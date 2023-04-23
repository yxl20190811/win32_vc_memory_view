#include "StdAfx.h"
#include "MyHeapDmp.h"
#include <process.h>
#include <stdio.h>
#include <malloc.h>

extern HMODULE hDllModule;
char ReportFileName[1024*100]={0};


typedef int     (__cdecl *THeapWalk)(_Inout_ _HEAPINFO * _EntryInfo);
THeapWalk CrtHeapWalk = NULL;


static class TInitMyHeapDmp
{
public:
	TInitMyHeapDmp()
	{
		
		CrtHeapWalk = (THeapWalk)(GetProcAddress(LoadLibrary(CrtDllName), "_heapwalk"));
	}
}InitMyHeapDmp;


extern "C"
__declspec( dllexport )
 int  WINAPI MyHeapDmp(const char* ptr)
{
	FILE*fp =fopen(ReportFileName,"wb");
	_HEAPINFO hinfo;
	int heapstatus;
	hinfo._pentry = NULL;
	char* ErrText = NULL;
	while(true)
	{
		heapstatus = CrtHeapWalk( &hinfo );
		switch( heapstatus )
		{
			case _HEAPOK:
				ErrText = NULL;
				break;
			case _HEAPEMPTY:
			  ErrText = "OK - empty heap\n";
			  break;
		   case _HEAPEND:
			  fclose(fp);
			  return 1;
		   case _HEAPBADPTR:
			  ErrText = "ERROR - bad pointer to heap\n";
			  break;
		   case _HEAPBADBEGIN:
			  ErrText = "ERROR - bad start of heap\n";
			  break;
		   case _HEAPBADNODE:
			  ErrText = "ERROR - bad node in heap\n";
			  break;
		}
		if(NULL != ErrText)
		{
			//::MessageBoxA(NULL, ErrText, "Î´Öª´íÎó",MB_ICONERROR);
			abort();
		}
		if(hinfo._useflag != _USEDENTRY)
		{
			//fprintf(fp, "%I64x\n", hinfo._pentry);
			continue;
		}
		TCallStack* info =(TCallStack*)((char*)hinfo._pentry + hinfo._size - sizeof(TCallStack));
		if(info->m_flag != CallStackFlag)
		{
			//fprintf(fp, "%I64x\n", hinfo._pentry);
			continue;
		}
		size_t size = hinfo._size - sizeof(TCallStack) - info->m_deep*sizeof(void*);
		fprintf(fp, "%I64x,%I64x,%u,",
			info->m_time, hinfo._pentry, size);
		void** CallStack =(void**)((char*)hinfo._pentry + size);
		for(int i=0;i<info->m_deep; ++i)
		{
			fprintf(fp, "%I64x,", CallStack[i]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return 1;
}

extern "C"
__declspec( dllexport )
void PrintHeapDmp()
{
	char FileName[1024+MAX_PATH];
	::GetModuleFileNameA(hDllModule, FileName, sizeof(FileName));
	char* pos = strrchr(FileName, '.');
	strcpy(pos, ".data");
	FILE* fp = fopen(FileName, "wb");

	fprintf(fp, "%I64u", (long long)(ReportFileName)-(long long)(hDllModule));
	fprintf(fp, "\n%I64u", ((long long)(MyHeapDmp)-(long long)(hDllModule)));
	fclose(fp);
}


