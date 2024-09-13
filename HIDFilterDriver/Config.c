#include "Trace.h"
#include "Config.tmh"
#include "Config.h"

NTSTATUS GetDriverConfig(IN WDFKEY Key) {
	NTSTATUS status = STATUS_SUCCESS;

	UNREFERENCED_PARAMETER(Key);

	// TODO retrieve driver configuration

	return status;
}
