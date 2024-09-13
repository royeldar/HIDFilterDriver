#include "Trace.h"
#include "Callbacks.tmh"
#include "Callbacks.h"

void PostReadCompleteCallback(IN WDFMEMORY OutputBuffer) {
	UNREFERENCED_PARAMETER(OutputBuffer);

	// TODO modify input report
}

void PreWriteSendCallback(IN WDFMEMORY InputBuffer) {
	UNREFERENCED_PARAMETER(InputBuffer);

	// TODO modify output report
}

void PreIoctlSendCallback(IN ULONG IoControlCode, IN WDFMEMORY InputBuffer) {
	UNREFERENCED_PARAMETER(IoControlCode);
	UNREFERENCED_PARAMETER(InputBuffer);

	// TODO modify output report
}

void PostIoctlCompleteCallback(IN ULONG IoControlCode, IN WDFMEMORY InputBuffer, IN WDFMEMORY OutputBuffer) {
	UNREFERENCED_PARAMETER(IoControlCode);
	UNREFERENCED_PARAMETER(InputBuffer);
	UNREFERENCED_PARAMETER(OutputBuffer);

	// TODO modify input report
}
