#include "Trace.h"
#include "Device.tmh"
#include "Device.h"

NTSTATUS MyEvtDeviceAdd(IN WDFDRIVER Driver, IN PWDFDEVICE_INIT DeviceInit) {
	NTSTATUS status = STATUS_SUCCESS;
	WDFDEVICE device;

	UNREFERENCED_PARAMETER(Driver);

	DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Adding device...");

	// Create a framework device object
	WdfFdoInitSetFilter(DeviceInit);
	status = WdfDeviceCreate(&DeviceInit, WDF_NO_OBJECT_ATTRIBUTES, &device);
	if (!NT_SUCCESS(status)) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to add device (0x%08x)!", status);
		return status;
	}

	DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Successfully added device.");

	return status;
}
