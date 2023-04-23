#pragma once

typedef size_t  (__cdecl *TQueryMallocSize)(void * _Memory);
extern TQueryMallocSize CrtQueryMallocSize;
extern TQueryMallocSize TrunkQueryMallocSize;

size_t  __cdecl MyQueryMallocSize(char* _Memor);


void InstallQueryMallocSize();
void UnstallQueryMallocSize();