#pragma once

#include <MinHook.h>
#include <process.h>

typedef void * (__cdecl *TMalloc)(_In_ size_t _Size);
extern TMalloc CrtMalloc;
extern TMalloc  TrunkMalloc;

void * __cdecl MyMalloc(_In_ size_t _Size);


void InstallMalloc();
void UnstallMalloc();
