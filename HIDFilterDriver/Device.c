#include "Trace.h"
#include "Device.tmh"
#include "Device.h"
#include "Events.h"

NTSTATUS MyEvtDeviceAdd(IN WDFDRIVER Driver, IN PWDFDEVICE_INIT DeviceInit) {
	NTSTATUS status = STATUS_SUCCESS;
	WDFDEVICE device;
	WDF_IO_QUEUE_CONFIG ioQueueConfig;

	UNREFERENCED_PARAMETER(Driver);

	DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Adding device...");

	// Create a framework device object
	WdfFdoInitSetFilter(DeviceInit);
	status = WdfDeviceCreate(&DeviceInit, WDF_NO_OBJECT_ATTRIBUTES, &device);
	if (!NT_SUCCESS(status)) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to add device (0x%08x)!", status);
		return status;
	}

	// Create an I/O queue for the device
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioQueueConfig, WdfIoQueueDispatchParallel);
	ioQueueConfig.EvtIoRead = MyEvtIoRead;
	ioQueueConfig.EvtIoWrite = MyEvtIoWrite;
	ioQueueConfig.EvtIoDeviceControl = MyEvtIoDeviceControl;
	status = WdfIoQueueCreate(device, &ioQueueConfig, WDF_NO_OBJECT_ATTRIBUTES, WDF_NO_HANDLE);
	if (!NT_SUCCESS(status)) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to create i/o queue (0x%08x)!", status);
		return status;
	}

	DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Successfully added device.");

	return status;
}
