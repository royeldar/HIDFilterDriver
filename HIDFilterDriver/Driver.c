#include "Trace.h"
#include "Driver.tmh"
#include "Driver.h"
#include "Device.h"
#include "Config.h"

static EVT_WDF_DRIVER_UNLOAD MyEvtDriverUnload;

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath) {
	NTSTATUS status = STATUS_SUCCESS;
	WDF_DRIVER_CONFIG config;
	WDFDRIVER driver;
	WDFKEY key;

	// Initialize WPP Tracing
	WPP_INIT_TRACING(DriverObject, RegistryPath);

	DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Initializing driver...");

	// Create a framework driver object
	WDF_DRIVER_CONFIG_INIT(&config, MyEvtDeviceAdd);
	config.EvtDriverUnload = MyEvtDriverUnload;
	status = WdfDriverCreate(DriverObject, RegistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, &driver);
	if (!NT_SUCCESS(status)) {
		// Cleanup tracing
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to create driver (0x%08x)!", status);
		WPP_CLEANUP(DriverObject);
		return status;
	}

	// Open the driver's parameters registry key
	status = WdfDriverOpenParametersRegistryKey(driver, KEY_READ, WDF_NO_OBJECT_ATTRIBUTES, &key);
	if (!NT_SUCCESS(status)) {
		// Cleanup tracing
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to open registry key (0x%08x)!", status);
		WPP_CLEANUP(DriverObject);
		return status;
	}

	// Retrieve driver-specific config from the registry
	status = GetDriverConfig(key);

	// Close the registry key
	WdfRegistryClose(key);

	if (!NT_SUCCESS(status)) {
		// Cleanup tracing
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to get driver parameters (0x%08x)!", status);
		WPP_CLEANUP(DriverObject);
		return status;
	}

	DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Successfully initialized driver.");

	return status;
}

static void MyEvtDriverUnload(IN WDFDRIVER Driver) {
	UNREFERENCED_PARAMETER(Driver);

	DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Unloading driver...");

	// Cleanup tracing
	WPP_CLEANUP(NULL);
}
