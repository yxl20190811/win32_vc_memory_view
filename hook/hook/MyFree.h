#pragma once
#include <MinHook.h>
#include <process.h>

typedef void   (__cdecl *TFree)(_Post_ptr_invalid_ void * _Memory);
extern TFree CrtFree;


