#pragma once

#include <Windows.h>
#include <wdf.h>

EVT_WDF_IO_QUEUE_IO_READ MyEvtIoRead;
EVT_WDF_IO_QUEUE_IO_WRITE MyEvtIoWrite;
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL MyEvtIoDeviceControl;
