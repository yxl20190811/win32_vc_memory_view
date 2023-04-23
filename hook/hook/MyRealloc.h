#pragma once


typedef void * (__cdecl *TRealloc)(_Post_ptr_invalid_ void * _Memory, _In_ size_t _NewSize);
extern TRealloc  CrtRealloc;
extern TRealloc  TrunkRealloc;

void InstallRealloc();
void UnstallRealloc();
