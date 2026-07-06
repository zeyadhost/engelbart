# Hardware Testing

Engelbart's first hardware target is Raspberry Pi Pico WH / RP2040 with Pico SDK and TinyUSB. Hardware automation is useful, but it should not become a build-system wrapper or a separate flashing product.

## USB-only Path

The simplest manual path is BOOTSEL mode:

- hold BOOTSEL while connecting or resetting the board
- build the Pico example with the Pico SDK toolchain
- copy a UF2 to the mounted RP2040 boot volume, when a UF2 is produced

`picotool` primarily interacts with RP2040/RP2350 devices in BOOTSEL mode. `picotool -f` can request a reboot from running firmware only when that firmware exposes compatible Pico SDK USB support. A HID-only TinyUSB device should not be assumed to support forced reboot.

## SWD/OpenOCD Path

For hands-off development and CI hardware testing, prefer SWD through a Raspberry Pi Debug Probe or Picoprobe with OpenOCD. This avoids depending on a BOOTSEL button press or on the running firmware exposing a reset-to-BOOTSEL path.

The repository does not currently provide flashing scripts. Any future script should be explicit about the connected probe, target board, firmware path, and whether it resets or flashes hardware.

## Current Test Boundary

Host tests cover descriptor generation, byte-exact fixtures, validation, allocation, inspection text, and TinyUSB-facing callback routing.

Hardware enumeration tests remain manual or CI-hardware-dependent. Do not claim full hardware automation until the board, probe, firmware format, and reset/flash path are verified in the target environment.
