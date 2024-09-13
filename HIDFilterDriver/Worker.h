#pragma once

#include <Windows.h>
#include <wdf.h>

#define WORKER_EXIT_TIMEOUT 50
#define WORKER_POLL_INTERVAL 1

extern volatile BOOL g_workerExit;

DWORD WINAPI MyWorkerThread(LPVOID lpParam);
