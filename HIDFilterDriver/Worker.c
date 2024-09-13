#include "Trace.h"
#include "Worker.tmh"
#include "Worker.h"

volatile BOOL g_workerExit = FALSE;

DWORD WINAPI MyWorkerThread(LPVOID lpParam) {
	UNREFERENCED_PARAMETER(lpParam);

	DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Starting worker thread...");

	while (!g_workerExit) {
		// TODO do work

		Sleep(WORKER_POLL_INTERVAL);
	}

	DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Exiting worker thread...");

	return 0;
}
