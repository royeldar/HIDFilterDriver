#include "Trace.h"
#include "Config.tmh"
#include "Config.h"
#include "Params.h"

static NTSTATUS GetRegistryString(IN WDFKEY Key, IN LPCWSTR name, OUT LPCWSTR *value);

NTSTATUS GetDriverConfig(IN WDFKEY Key) {
	NTSTATUS status = STATUS_SUCCESS;

	// Retrieve serial port name
	status = GetRegistryString(Key, L"SerialPortName", &g_serialportname);
	if (!NT_SUCCESS(status)) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to get registry string for %S (0x%08x)!", L"SerialPortName", status);
		return status;
	}

	// Retrieve serial port configuration
	status = GetRegistryString(Key, L"SerialPortConfig", &g_serialportconfig);
	if (!NT_SUCCESS(status)) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to get registry string for %S (0x%08x)!", L"SerialPortConfig", status);
		return status;
	}

	DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] %S = %S", L"SerialPortName", g_serialportname);
	DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] %S = %S", L"SerialPortConfig", g_serialportconfig);

	return status;
}

static NTSTATUS GetRegistryString(IN WDFKEY Key, IN LPCWSTR name, OUT LPCWSTR *value) {
	NTSTATUS status = STATUS_SUCCESS;
	WDFSTRING string;
	UNICODE_STRING _name;
	UNICODE_STRING _value;

	// Create a framework string object
	status = WdfStringCreate(NULL, WDF_NO_OBJECT_ATTRIBUTES, &string);
	if (!NT_SUCCESS(status)) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to create string object (0x%08x)!", status);
		return status;
	}

	// Query the registry
	RtlInitUnicodeString(&_name, name);
	status = WdfRegistryQueryString(Key, &_name, string);
	if (!NT_SUCCESS(status)) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to query registry (0x%08x)!", status);
		return status;
	}

	WdfStringGetUnicodeString(string, &_value);

	// Validate string is null-terminated
	if (_value.MaximumLength < _value.Length + 2 ||
		_value.Length % 2 != 0 ||
		_value.Buffer[_value.Length / 2] != L'\0')
	{
		status = STATUS_UNSUCCESSFUL;
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] String is not null terminated!");
		return status;
	}

	*value = _value.Buffer;

	return status;
}
