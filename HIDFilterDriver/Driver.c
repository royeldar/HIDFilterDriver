#include "Driver.h"
#include "Device.h"

static EVT_WDF_DRIVER_UNLOAD MyEvtDriverUnload;

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath) {
	NTSTATUS status = STATUS_SUCCESS;
	WDF_DRIVER_CONFIG config;
	WDFDRIVER driver;

	// Create a framework driver object
	WDF_DRIVER_CONFIG_INIT(&config, MyEvtDeviceAdd);
	config.EvtDriverUnload = MyEvtDriverUnload;
	status = WdfDriverCreate(DriverObject, RegistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, &driver);

	return status;
}

static void MyEvtDriverUnload(IN WDFDRIVER Driver) {
	UNREFERENCED_PARAMETER(Driver);
}
