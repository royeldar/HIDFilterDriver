#include "Trace.h"
#include "Device.tmh"
#include "Device.h"

NTSTATUS MyEvtDeviceAdd(IN WDFDRIVER Driver, IN PWDFDEVICE_INIT DeviceInit) {
	NTSTATUS status = STATUS_SUCCESS;

	UNREFERENCED_PARAMETER(Driver);
	UNREFERENCED_PARAMETER(DeviceInit);

	// TODO add device

	return status;
}
