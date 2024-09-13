#include "Trace.h"
#include "Worker.tmh"
#include "Worker.h"
#include "Input.h"
#include "Params.h"

volatile BOOL g_workerExit = FALSE;

static HANDLE OpenSerialPort(LPCWSTR portName);

DWORD WINAPI MyWorkerThread(LPVOID lpParam) {
	static HANDLE file = INVALID_HANDLE_VALUE;

	UNREFERENCED_PARAMETER(lpParam);

	DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Starting worker thread...");

	while (!g_workerExit) {
		if (file == INVALID_HANDLE_VALUE) {
			// Try to open serial port
			file = OpenSerialPort(g_serialportname);
		}
		if (file != INVALID_HANDLE_VALUE) {
			// Try to read bytes from serial
			BYTE c;
			DWORD bytesRead;
			if (!ReadFile(file, &c, 1, &bytesRead, NULL)) {
				// Close serial port
				DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to read from serial port (0x%x)!", GetLastError());
				if (!CloseHandle(file)) {
					DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to close file handle (0x%x)!", GetLastError());
				}
				file = INVALID_HANDLE_VALUE;
			} else if (bytesRead > 0) {
				// Update x-axis input and continue
				g_xaxis = c;
				continue;
			} else {
				// Check whether serial port is still connected
				DWORD modemStatus;
				if (!GetCommModemStatus(file, &modemStatus)) {
					// Close serial port
					DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to retrieve serial port state (0x%x)!", GetLastError());
					if (!CloseHandle(file)) {
						DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to close file handle (0x%x)!", GetLastError());
					}
					file = INVALID_HANDLE_VALUE;
				}
			}
		}
		Sleep(WORKER_POLL_INTERVAL);
	}

	DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Exiting worker thread...");

	if (file != INVALID_HANDLE_VALUE) {
		if (!CloseHandle(file)) {
			DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to close file handle (0x%x)!", GetLastError());
		}
	}

	return 0;
}

static HANDLE OpenSerialPort(LPCWSTR portName) {
	HANDLE file;
	COMMTIMEOUTS commTimeouts;
	DCB dcb;

	DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Opening serial port...");

	// Open serial port
	file = CreateFileW(portName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (file == INVALID_HANDLE_VALUE) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to open serial port (0x%x)!", GetLastError());
		goto error;
	}

	// Set timeout parameters
	commTimeouts.ReadIntervalTimeout = MAXDWORD;
	commTimeouts.ReadTotalTimeoutConstant = 0;
	commTimeouts.ReadTotalTimeoutMultiplier = 0;
	commTimeouts.WriteTotalTimeoutConstant = 0;
	commTimeouts.WriteTotalTimeoutMultiplier = 0;
	if (!SetCommTimeouts(file, &commTimeouts)) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to set timeout parameters (0x%x)!", GetLastError());
		goto error;
	}

	// Build control settings
	memset(&dcb, 0, sizeof(dcb));
	if (!BuildCommDCBW(g_serialportconfig, &dcb)) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to build communication control settings (0x%x)!", GetLastError());
		goto error;
	}

	// Set control settings
	if (!SetCommState(file, &dcb)) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to set control settings (0x%x)!", GetLastError());
		goto error;
	}

	// Empty input queue
	if (!PurgeComm(file, PURGE_RXCLEAR)) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to empty input queue (0x%x)!", GetLastError());
		goto error;
	}

	return file;

error:
	// Close file handle
	if (file != INVALID_HANDLE_VALUE && !CloseHandle(file)) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to close file handle (0x%x)!", GetLastError());
	}
	return INVALID_HANDLE_VALUE;
}
