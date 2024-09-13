#pragma once

#include <Windows.h>
#include <wdf.h>

#define WPP_CONTROL_GUIDS \
	WPP_DEFINE_CONTROL_GUID(MyDriverTraceGuid, (96e5532e,167d,4497,8453,846ab800ada9),  \
		WPP_DEFINE_BIT(TRACE_DRIVER) \
	)
