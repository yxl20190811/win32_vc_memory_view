// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <malloc.h>
#include <Windows.h>
#include <conio.h>

void  heapdump(char* FileName)
{
	FILE*fp =fopen(FileName,"wb");
	_HEAPINFO hinfo;
	int heapstatus;
	hinfo._pentry = NULL;
	char* ErrText = NULL;
	while(true)
	{
		heapstatus = _heapwalk( &hinfo );
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
			  return;
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
			//::MessageBoxA(NULL, ErrText, "未知错误",MB_ICONERROR);
			abort();
		}
		fprintf(fp, "%I64x\n", hinfo._pentry);
	}
	fclose(fp);
}

int _tmain(int argc, _TCHAR* argv[])
{
	for(int i = 0; i < 10000*100;++i)
	{
		char* ptr1 = (char*)malloc(100);
	}
	

	while(true)
	{
		Sleep(100);
	}
	return 0;
}

