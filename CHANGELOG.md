# Changelog

## v0.1.1 - 2026-07-08

Hardening changes:

- Additional compile-failure validation coverage.
- Additional string descriptor edge-case tests.
- Additional descriptor invariant checks for supported HID layouts.

## v0.1.0 - 2026-07-07

This is an early preview of Engelbart as a compile-time USB device compiler for embedded C++.

Included:

- HID mouse descriptor generation
- boot keyboard descriptor generation
- composite mouse + keyboard descriptor generation with separate HID interfaces
- deterministic USB device, configuration, string, endpoint, and HID descriptor bytes
- HID report descriptors for the supported mouse and keyboard models
- endpoint and interface allocation for the supported devices
- printable ASCII USB string descriptors encoded as UTF-16LE
- TinyUSB-style descriptor glue used by the Pico smoke example
- byte-exact golden tests and validation tests
- focused compile-failure tests for selected invalid device descriptions
- Raspberry Pi Pico WH / RP2040 TinyUSB smoke firmware
- documented picotool BOOTSEL workflow for the smoke firmware
- host-side GitHub Actions CI

Not included:

- CDC
- MIDI
- gamepad
- vendor class support as an Engelbart-generated device class
- CLI
- GUI
- dashboard
- Studio
- Inspector
- runtime descriptor interpreter
- runtime descriptor reconfiguration
- multi-board support

The public API, diagnostic IDs, and inspection text remain pre-1.0 and may change.
