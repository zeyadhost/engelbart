# Hardware Testing

Engelbart's first hardware target is Raspberry Pi Pico WH / RP2040 with Pico SDK and TinyUSB. Hardware automation is useful, but it should not become a build-system wrapper or a separate flashing product.

## USB-only Path

The simplest manual path is BOOTSEL mode:

- hold BOOTSEL while connecting or resetting the board
- build the Pico example with the Pico SDK toolchain
- copy a UF2 to the mounted RP2040 boot volume, when a UF2 is produced

`picotool` primarily interacts with RP2040/RP2350 devices in BOOTSEL mode. `picotool -f` can request a reboot from running firmware only when that firmware exposes compatible Pico SDK USB support. A HID-only TinyUSB device should not be assumed to support forced reboot.

The Pico TinyUSB smoke firmware is development-friendly: it emits UF2/bin/hex outputs, appends the Pico SDK reset vendor interface to the smoke firmware's USB configuration, and exposes a static serial string from the Pico SDK unique board ID so picotool can track the device across forced reboots. This reset and serial support is smoke-test harness infrastructure, not an Engelbart-generated USB class.

Typical command-line workflow:

```sh
cmake -S examples/pico_tinyusb_smoke -B build-pico-smoke -DPICO_BOARD=pico_w
cmake --build build-pico-smoke
picotool load -x build-pico-smoke/engelbart_pico_tinyusb_smoke.uf2
picotool info -a -f --vid 0x1209 --pid 0x0003
picotool reboot -u -f --vid 0x1209 --pid 0x0003
```

After `picotool reboot -u -f --vid 0x1209 --pid 0x0003`, the board should appear in BOOTSEL mode and can be loaded again with `picotool load -x`.

The explicit VID/PID arguments matter. The smoke firmware intentionally uses the pid.codes-style test VID/PID `1209:0003`. With picotool 2.2.0, bare `picotool -f` defaults to Raspberry Pi VID filtering and does not inspect this third-party VID/PID for a reset interface. Do not change the smoke firmware to use Raspberry Pi's VID/PID just to inherit default picotool filtering. Engelbart examples should not impersonate real USB products.

On Linux, `picotool -f` also needs permission to open the running USB device. Default Raspberry Pi `2e8a` udev rules do not apply to `1209:0003`. If an explicit command such as `picotool info -a -f --vid 0x1209 --pid 0x0003` reports that the device is present but inaccessible, install a local udev rule for the smoke VID/PID, reload udev rules, and reconnect or re-enumerate the board:

```udev
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1209", ATTRS{idProduct}=="0003", MODE="0660", TAG+="uaccess"
```

Permissions are necessary, but they are not sufficient: the running firmware must also expose the reset interface and a usable serial string. The Pico smoke firmware does both. Other Engelbart-generated HID descriptors do not imply picotool forced reboot support.

## SWD/OpenOCD Path

For hands-off development and CI hardware testing, prefer SWD through a Raspberry Pi Debug Probe or Picoprobe with OpenOCD. This avoids depending on a BOOTSEL button press or on the running firmware exposing a reset-to-BOOTSEL path.

The repository does not currently provide flashing scripts. Any future script should be explicit about the connected probe, target board, firmware path, and whether it resets or flashes hardware.

## Current Test Boundary

Host tests cover descriptor generation, byte-exact fixtures, validation, allocation, inspection text, and TinyUSB-facing callback routing.

Hardware enumeration tests remain manual or CI-hardware-dependent. Do not claim full hardware automation until the board, probe, firmware format, and reset/flash path are verified in the target environment.
