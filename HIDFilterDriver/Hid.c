#include "Trace.h"
#include "Hid.tmh"
#include "Hid.h"
#include "Input.h"

#define INPUT_REPORT_OFFSET 3

void ModifyOutputReport(char *buffer, size_t size) {
	(void)buffer;
	(void)size;

	// TODO modify force feedback
}

void ModifyInputReport(char *buffer, size_t size) {
	// overwrite x-axis input
	if (INPUT_REPORT_OFFSET < size) {
		buffer[INPUT_REPORT_OFFSET] = g_xaxis;
	}
}
