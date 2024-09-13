#pragma once

#include <Windows.h>
#include <wdf.h>

NTSTATUS GetDriverConfig(IN WDFKEY Key);
