# MSC
Mass Storage Controller for Teensy T3.6 and T4.0

This is a USB Mass Storage driver based on PJRC's USBHost_t36 library.

It is still very much WIP.
You will need Arduino version 1.89 and Teensyduino version 1.47 Beta 2 from PJRC.
Currently there is no example programs within MSC itself. For examples of the use of MSC
you need to use example programs found in WMXZ's uSDFS library which implements MSC.

Teensyduino 1.47Beta2 found here in post #2:
https://forum.pjrc.com/threads/54711-Teensy-4-0-First-Beta-Test

WMXZ uSDFS found here: https://github.com/WMXZ-EU/uSDFS

PJRC forum thread for MSC: https://forum.pjrc.com/threads/55821-USBHost_t36-USB-Mass-Storage-Driver-Experiments

Updated 07/28/19: Added more complete error processing using sense codes.
