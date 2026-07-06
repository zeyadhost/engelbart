# Hardware Testing

Engelbart's first hardware target is Raspberry Pi Pico WH / RP2040 with Pico SDK and TinyUSB. Hardware automation is useful, but it should not become a build-system wrapper or a separate flashing product.

## USB-only Path

The simplest manual path is BOOTSEL mode:

- hold BOOTSEL while connecting or resetting the board
- build the Pico example with the Pico SDK toolchain
- copy a UF2 to the mounted RP2040 boot volume, when a UF2 is produced

`picotool` primarily interacts with RP2040/RP2350 devices in BOOTSEL mode. `picotool -f` can request a reboot from running firmware only when that firmware exposes compatible Pico SDK USB support. A HID-only TinyUSB device should not be assumed to support forced reboot.

The Pico TinyUSB smoke firmware is development-friendly: it emits UF2/bin/hex outputs, appends the Pico SDK reset vendor interface to the smoke firmware's USB configuration, and exposes a static serial string from the Pico SDK unique board ID so picotool can track the device across forced reboots. This reset and serial support is smoke-test harness infrastructure, not an Engelbart-generated USB class.

The smoke firmware uses the pid.codes-style test VID/PID `1209:0003`. RP2040 BOOTSEL mode uses Raspberry Pi VID/PID `2e8a:0003`. These are different USB devices from the host's point of view.

## Verified Picotool Workflow

Build the smoke firmware:

```sh
cmake -S examples/pico_tinyusb_smoke -B build-pico-smoke -DPICO_BOARD=pico_w
cmake --build build-pico-smoke
```

When the board is running the smoke firmware, it should appear as `1209:0003`:

```sh
lsusb | grep -Ei '2e8a|1209|raspberry|rp2|pico'
```

To ask the running smoke firmware to reboot into BOOTSEL, target the smoke VID/PID explicitly:

```sh
sudo picotool reboot -u -f --vid 0x1209 --pid 0x0003 || true
```

`picotool info -a -f --vid 0x1209 --pid 0x0003` also sends the forced reboot request before trying to inspect the rebooted BOOTSEL device. On tested hardware, picotool 2.2.0 successfully asked the firmware to reboot but then printed a misleading failure because it did not find the rebooted BOOTSEL device by the tracked serial:

```text
Despite the reboot attempt, no accessible RP-series devices in BOOTSEL mode were found found with serial number ...
```

If that happens, check USB enumeration directly:

```sh
lsusb | grep -Ei '2e8a|1209|raspberry|rp2|pico'
```

If the reset worked, the board appears as `2e8a:0003 Raspberry Pi RP2 Boot`. Once it is in BOOTSEL, stop using the smoke firmware VID/PID filters. BOOTSEL is `2e8a:0003`, not `1209:0003`.

Inspect and flash the BOOTSEL device:

```sh
sudo picotool info -a
sudo picotool load -x build-pico-smoke/engelbart_pico_tinyusb_smoke.uf2
```

After `picotool load -x`, the board should reboot into the smoke firmware and appear again as `1209:0003 Generic pid.codes Test PID`.

The explicit VID/PID arguments matter. The smoke firmware intentionally uses the pid.codes-style test VID/PID `1209:0003`. With picotool 2.2.0, bare `picotool -f` defaults to Raspberry Pi VID filtering and does not inspect this third-party VID/PID for a reset interface. Do not change the smoke firmware to use Raspberry Pi's VID/PID just to inherit default picotool filtering. Engelbart examples should not impersonate real USB products.

On Linux, `picotool -f` also needs permission to open the running USB device. Default Raspberry Pi `2e8a` udev rules do not apply to `1209:0003`. Use `sudo` or install a local udev rule for the smoke VID/PID, reload udev rules, and reconnect or re-enumerate the board:

```udev
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1209", ATTRS{idProduct}=="0003", MODE="0660", TAG+="uaccess"
```

Permissions are necessary, but they are not sufficient: the running firmware must also expose the reset interface and a usable serial string. The Pico smoke firmware does both. Other Engelbart-generated HID descriptors do not imply picotool forced reboot support. This workflow is only for the Pico smoke-test firmware and does not make CDC a supported Engelbart-generated USB class.

## SWD/OpenOCD Path

For hands-off development and CI hardware testing, prefer SWD through a Raspberry Pi Debug Probe or Picoprobe with OpenOCD. This avoids depending on a BOOTSEL button press or on the running firmware exposing a reset-to-BOOTSEL path.

The repository does not currently provide flashing scripts. Any future script should be explicit about the connected probe, target board, firmware path, and whether it resets or flashes hardware.

## Current Test Boundary

Host tests cover descriptor generation, byte-exact fixtures, validation, allocation, inspection text, and TinyUSB-facing callback routing.

Hardware enumeration tests remain manual or CI-hardware-dependent. Do not claim full hardware automation until the board, probe, firmware format, and reset/flash path are verified in the target environment.
