# HID Filter Driver

This is a simple UMDF2 driver which is installed as a filter driver on top of the HID stack.

It then registers callbacks on I/O events, so that it can modify HID input/output reports.

Whenever a "Logic3 Topdrive GT" steering wheel is connected, the driver
creates a filter device object which is attached to the HID stack; from there,
it's essentially man-in-the-middle for HID input/output reports, which is used to
inject more accurate readings of the x-axis potentiometer into the HID input reports.

The x-axis input is continuously polled by a driver thread, which reads from
a serial port communication resource (COM port); the actual input is being fed
to the serial port via an Arduino.

# Arduino Program

This is a very simple program, which reads from the analog input and writes to the serial;
it's assumed that the ADC resolution is 10-bits (that should be the default).

```c
void setup() {
  // 8 data bits, no parity, one stop bit
  Serial.begin(9600);
}

void loop() {
  int val = analogRead(A1); // 0 - 1023
  Serial.write((uint8_t)(val / 4)); // 0 - 255
  Serial.flush();
}
```

# Installation

In order to install the driver, do the following:

1. add the test certificate to "Local Computer\Trusted Root Certification Authorities\Certificates"
2. install the driver, using the INF file

In order to configure the serial port parameters, do the following:

1. go to the following registry key: "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\WUDF\Services\HIDFilterDriver\Parameters"
2. modify "SerialPortName" and "SerialPortConfig" appropriately

In order to uninstall the driver, do the following:

1. run the following command: "pnputil /delete-driver C:\Windows\System32\DriverStore\FileRepository\hidfilterdriver.inf_amd64_* /uninstall"
2. optionally, you can also delete the test certificate added before

You should connect the gamepad device after the driver is installed and the serial port parameters are configured;
preferably, connect the arduino before the gamepad, although it shouldn't matter too much.

You should disconnect the gamepad device before attempting to uninstall the driver, otherwise it might not work.
