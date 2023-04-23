#include "StdAfx.h"
#include "MyFree.h"

TFree CrtFree = NULL;

static class TInitFree
{
public:
	TInitFree()
	{
		CrtFree = (TFree)GetProcAddress(LoadLibrary(CrtDllName), "free");
	}
	~TInitFree()
	{
	}
}InitFree;
