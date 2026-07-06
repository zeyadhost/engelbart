# Hardware Testing

Engelbart's first hardware target is Raspberry Pi Pico WH / RP2040 with Pico SDK and TinyUSB. Hardware automation is useful, but it should not become a build-system wrapper or a separate flashing product.

## USB-only Path

The simplest manual path is BOOTSEL mode:

- hold BOOTSEL while connecting or resetting the board
- build the Pico example with the Pico SDK toolchain
- copy a UF2 to the mounted RP2040 boot volume, when a UF2 is produced

`picotool` primarily interacts with RP2040/RP2350 devices in BOOTSEL mode. `picotool -f` can request a reboot from running firmware only when that firmware exposes compatible Pico SDK USB support. A HID-only TinyUSB device should not be assumed to support forced reboot.

The Pico TinyUSB smoke firmware is development-friendly: it emits UF2/bin/hex outputs and appends the Pico SDK reset vendor interface to the smoke firmware's USB configuration so `picotool reboot -f -u` can reset the running board into BOOTSEL mode. This reset interface is a smoke-test harness feature, not an Engelbart-generated USB class.

Typical command-line workflow:

```sh
cmake -S examples/pico_tinyusb_smoke -B build-pico-smoke -DPICO_BOARD=pico_w
cmake --build build-pico-smoke
picotool load -x build-pico-smoke/engelbart_pico_tinyusb_smoke.uf2
picotool info -a -f
picotool reboot -f -u
```

After `picotool reboot -f -u`, the board should appear in BOOTSEL mode and can be loaded again with `picotool load -x`.

On Linux, `picotool -f` also needs permission to open the running USB device. The smoke firmware intentionally uses the pid.codes-style test VID/PID `1209:0003`, so default Raspberry Pi `2e8a` udev rules may not apply. If `picotool info -a -f` reports no accessible device while `lsusb` shows `1209:0003`, install a local udev rule for the smoke VID/PID, reload udev rules, and reconnect or re-enumerate the board:

```udev
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1209", ATTRS{idProduct}=="0003", MODE="0660", TAG+="uaccess"
```

Do not change the smoke firmware to use Raspberry Pi's VID/PID just to inherit existing udev rules. Engelbart examples should not impersonate real USB products.

## SWD/OpenOCD Path

For hands-off development and CI hardware testing, prefer SWD through a Raspberry Pi Debug Probe or Picoprobe with OpenOCD. This avoids depending on a BOOTSEL button press or on the running firmware exposing a reset-to-BOOTSEL path.

The repository does not currently provide flashing scripts. Any future script should be explicit about the connected probe, target board, firmware path, and whether it resets or flashes hardware.

## Current Test Boundary

Host tests cover descriptor generation, byte-exact fixtures, validation, allocation, inspection text, and TinyUSB-facing callback routing.

Hardware enumeration tests remain manual or CI-hardware-dependent. Do not claim full hardware automation until the board, probe, firmware format, and reset/flash path are verified in the target environment.
