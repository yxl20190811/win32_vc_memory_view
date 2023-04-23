#include "StdAfx.h"
#include "MyCaptureCallStack.h"

TCaptureCallStack CrtCaptureCallStack = NULL;


static class TInitCaptureCallStack
{
public:
	TInitCaptureCallStack()
	{
		CrtCaptureCallStack = (TCaptureCallStack)(GetProcAddress(LoadLibrary("kernel32.dll"), "RtlCaptureStackBackTrace"));
	}
}InitCaptureCallStac;
