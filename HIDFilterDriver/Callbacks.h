#pragma once

#include <Windows.h>
#include <wdf.h>

void PostReadCompleteCallback(IN WDFMEMORY OutputBuffer);
void PreWriteSendCallback(IN WDFMEMORY InputBuffer);
void PreIoctlSendCallback(IN ULONG IoControlCode, IN WDFMEMORY InputBuffer);
void PostIoctlCompleteCallback(IN ULONG IoControlCode, IN WDFMEMORY InputBuffer, IN WDFMEMORY OutputBuffer);
