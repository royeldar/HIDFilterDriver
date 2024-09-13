#include "Trace.h"
#include "Events.tmh"
#include "Events.h"
#include "Callbacks.h"

static EVT_WDF_REQUEST_COMPLETION_ROUTINE MyEvtIoReadRequestCompletionRoutine;
static EVT_WDF_REQUEST_COMPLETION_ROUTINE MyEvtIoWriteRequestCompletionRoutine;
static EVT_WDF_REQUEST_COMPLETION_ROUTINE MyEvtIoctlRequestCompletionRoutine;

void MyEvtIoRead(IN WDFQUEUE Queue, IN WDFREQUEST Request, IN size_t Length) {
	NTSTATUS status = STATUS_SUCCESS;
	WDFDEVICE device;
	WDFIOTARGET target;
	WDFMEMORY buffer;
	BOOLEAN ret;

	UNREFERENCED_PARAMETER(Length);

	// Retrieve the output buffer of the I/O request
	status = WdfRequestRetrieveOutputMemory(Request, &buffer);
	if (!NT_SUCCESS(status)) {
		if (status == STATUS_BUFFER_TOO_SMALL) {
			buffer = NULL;
		}
		else {
			DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to retrieve output buffer (0x%08x)!", status);
			WdfRequestComplete(Request, status);
			return;
		}
	}

	// Get the I/O target for the device
	device = WdfIoQueueGetDevice(Queue);
	target = WdfDeviceGetIoTarget(device);

	// Prepare to forward the I/O request
	status = WdfIoTargetFormatRequestForRead(target, Request, buffer, NULL, NULL);
	if (!NT_SUCCESS(status)) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to format i/o request (0x%08x)!", status);
		WdfRequestComplete(Request, status);
		return;
	}

	// Send the I/O request
	WdfRequestSetCompletionRoutine(Request, MyEvtIoReadRequestCompletionRoutine, WDF_NO_CONTEXT);
	ret = WdfRequestSend(Request, target, WDF_NO_SEND_OPTIONS);
	if (!ret) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to send i/o request (0x%08x)!", status);
		status = WdfRequestGetStatus(Request);
		WdfRequestComplete(Request, status);
	}
}

static void MyEvtIoReadRequestCompletionRoutine(IN WDFREQUEST Request, IN WDFIOTARGET Target, IN PWDF_REQUEST_COMPLETION_PARAMS Params, IN WDFCONTEXT Context) {
	NTSTATUS status;
	WDFMEMORY buffer;

	UNREFERENCED_PARAMETER(Target);
	UNREFERENCED_PARAMETER(Context);

	status = Params->IoStatus.Status;
	buffer = Params->Parameters.Read.Buffer;

	// Hook after completing the I/O request
	PostReadCompleteCallback(buffer);

	// Complete the I/O request
	WdfRequestComplete(Request, status);
}

void MyEvtIoWrite(IN WDFQUEUE Queue, IN WDFREQUEST Request, IN size_t Length) {
	NTSTATUS status = STATUS_SUCCESS;
	WDFDEVICE device;
	WDFIOTARGET target;
	WDFMEMORY buffer;
	BOOLEAN ret;

	UNREFERENCED_PARAMETER(Length);

	// Retrieve the input buffer of the I/O request
	status = WdfRequestRetrieveInputMemory(Request, &buffer);
	if (!NT_SUCCESS(status)) {
		if (status == STATUS_BUFFER_TOO_SMALL) {
			buffer = NULL;
		}
		else {
			DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to retrieve output buffer (0x%08x)!", status);
			WdfRequestComplete(Request, status);
			return;
		}
	}

	// Hook before sending the I/O request
	PreWriteSendCallback(buffer);

	// Get the I/O target for the device
	device = WdfIoQueueGetDevice(Queue);
	target = WdfDeviceGetIoTarget(device);

	// Prepare to forward the I/O request
	status = WdfIoTargetFormatRequestForWrite(target, Request, buffer, NULL, NULL);
	if (!NT_SUCCESS(status)) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to format i/o request (0x%08x)!", status);
		WdfRequestComplete(Request, status);
		return;
	}

	// Send the I/O request
	WdfRequestSetCompletionRoutine(Request, MyEvtIoWriteRequestCompletionRoutine, WDF_NO_CONTEXT);
	ret = WdfRequestSend(Request, target, WDF_NO_SEND_OPTIONS);
	if (!ret) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to send i/o request (0x%08x)!", status);
		status = WdfRequestGetStatus(Request);
		WdfRequestComplete(Request, status);
	}
}

static void MyEvtIoWriteRequestCompletionRoutine(IN WDFREQUEST Request, IN WDFIOTARGET Target, IN PWDF_REQUEST_COMPLETION_PARAMS Params, IN WDFCONTEXT Context) {
	NTSTATUS status;

	UNREFERENCED_PARAMETER(Target);
	UNREFERENCED_PARAMETER(Context);

	status = Params->IoStatus.Status;

	// Complete the I/O request
	WdfRequestComplete(Request, status);
}

void MyEvtIoDeviceControl(IN WDFQUEUE Queue, IN WDFREQUEST Request, IN size_t OutputBufferLength, IN size_t InputBufferLength, IN ULONG IoControlCode) {
	NTSTATUS status = STATUS_SUCCESS;
	WDFDEVICE device;
	WDFIOTARGET target;
	WDFMEMORY inputBuffer;
	WDFMEMORY outputBuffer;
	BOOLEAN ret;

	UNREFERENCED_PARAMETER(InputBufferLength);
	UNREFERENCED_PARAMETER(OutputBufferLength);

	// Retrieve the input buffer of the I/O request
	status = WdfRequestRetrieveInputMemory(Request, &inputBuffer);
	if (!NT_SUCCESS(status)) {
		if (status == STATUS_BUFFER_TOO_SMALL) {
			inputBuffer = NULL;
		} else {
			DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to retrieve input buffer (0x%08x)!", status);
			WdfRequestComplete(Request, status);
			return;
		}
	}

	// Retrieve the output buffer of the I/O request
	status = WdfRequestRetrieveOutputMemory(Request, &outputBuffer);
	if (!NT_SUCCESS(status)) {
		if (status == STATUS_BUFFER_TOO_SMALL) {
			outputBuffer = NULL;
		} else {
			DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to retrieve output buffer (0x%08x)!", status);
			WdfRequestComplete(Request, status);
			return;
		}
	}

	// Hook before sending the I/O request
	PreIoctlSendCallback(IoControlCode, inputBuffer);

	// Get the I/O target for the device
	device = WdfIoQueueGetDevice(Queue);
	target = WdfDeviceGetIoTarget(device);

	// Prepare to forward the I/O request
	status = WdfIoTargetFormatRequestForIoctl(target, Request, IoControlCode, inputBuffer, NULL, outputBuffer, NULL);
	if (!NT_SUCCESS(status)) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to format i/o request (0x%08x)!", status);
		WdfRequestComplete(Request, status);
		return;
	}

	// Send the I/O request
	WdfRequestSetCompletionRoutine(Request, MyEvtIoctlRequestCompletionRoutine, WDF_NO_CONTEXT);
	ret = WdfRequestSend(Request, target, WDF_NO_SEND_OPTIONS);
	if (!ret) {
		DoTraceMessage(TRACE_DRIVER, "[%!FUNC!] Failed to send i/o request (0x%08x)!", status);
		status = WdfRequestGetStatus(Request);
		WdfRequestComplete(Request, status);
	}
}

static void MyEvtIoctlRequestCompletionRoutine(IN WDFREQUEST Request, IN WDFIOTARGET Target, IN PWDF_REQUEST_COMPLETION_PARAMS Params, IN WDFCONTEXT Context) {
	NTSTATUS status;
	ULONG ioControlCode;
	WDFMEMORY inputBuffer;
	WDFMEMORY outputBuffer;

	UNREFERENCED_PARAMETER(Target);
	UNREFERENCED_PARAMETER(Context);

	status = Params->IoStatus.Status;
	ioControlCode = Params->Parameters.Ioctl.IoControlCode;
	inputBuffer = Params->Parameters.Ioctl.Input.Buffer;
	outputBuffer = Params->Parameters.Ioctl.Output.Buffer;

	// Hook after completing the I/O request
	PostIoctlCompleteCallback(ioControlCode, inputBuffer, outputBuffer);

	// Complete the I/O request
	WdfRequestComplete(Request, status);
}
