#include "Trace.h"
#include "Callbacks.tmh"
#include "Callbacks.h"
#include "Hid.h"

#include <hidclass.h>

void PostReadCompleteCallback(IN WDFMEMORY OutputBuffer) {
	char *buffer = NULL;
	size_t size = 0;
	if (OutputBuffer != NULL)
		buffer = WdfMemoryGetBuffer(OutputBuffer, &size);

	// Modify the output buffer of the I/O request
	ModifyInputReport(buffer, size);
}

void PreWriteSendCallback(IN WDFMEMORY InputBuffer) {
	char *buffer = NULL;
	size_t size = 0;
	if (InputBuffer != NULL)
		buffer = WdfMemoryGetBuffer(InputBuffer, &size);

	// Modify the input buffer of the I/O request
	ModifyOutputReport(buffer, size);
}

void PreIoctlSendCallback(IN ULONG IoControlCode, IN WDFMEMORY InputBuffer) {
	char *buffer = NULL;
	size_t size = 0;
	if (InputBuffer != NULL)
		buffer = WdfMemoryGetBuffer(InputBuffer, &size);

	// Modify the input buffer of the I/O request
	if (IoControlCode == IOCTL_HID_SET_OUTPUT_REPORT)
		ModifyOutputReport(buffer, size);
}

void PostIoctlCompleteCallback(IN ULONG IoControlCode, IN WDFMEMORY InputBuffer, IN WDFMEMORY OutputBuffer) {
	UNREFERENCED_PARAMETER(InputBuffer);

	char *buffer = NULL;
	size_t size = 0;
	if (OutputBuffer != NULL)
		buffer = WdfMemoryGetBuffer(OutputBuffer, &size);

	// Modify the output buffer of the I/O request
	if (IoControlCode == IOCTL_HID_GET_INPUT_REPORT)
		ModifyInputReport(buffer, size);
}
