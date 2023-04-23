#pragma once

typedef USHORT (WINAPI *TCaptureCallStack)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);
extern TCaptureCallStack CrtCaptureCallStack;


