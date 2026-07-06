# Engelbart

Engelbart is a compile-time USB device compiler for embedded C++.

Users describe a USB device with a high-level, strongly typed C++ API. Engelbart generates USB descriptors, HID report descriptors, endpoint layouts, interface numbering, composite device configuration, TinyUSB integration, compile-time validation, and human-readable diagnostics.

The first target is RP2040 / Raspberry Pi Pico WH with Pico SDK and TinyUSB. The current implementation supports the minimal year-one HID model: mouse, boot keyboard, and composite mouse + keyboard with separate HID interfaces.

## Build

Host tests and examples:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Pico WH / TinyUSB smoke build, when `PICO_SDK_PATH` and `arm-none-eabi-g++` are available:

```sh
cmake -S examples/pico_tinyusb_smoke -B build-pico-smoke -DPICO_BOARD=pico_w
cmake --build build-pico-smoke
```

Start with:

- [Engineering principles](docs/principles.md)
- [Non-goals](docs/non-goals.md)
- [Roadmap](docs/roadmap.md)
- [ADR 0001: Project Identity](docs/adr/0001-project-identity.md)
- [RFC 0001: Minimal HID Device Model](docs/rfc/0001-minimal-hid-device-model.md)
- [RFC 0002: Minimal HID Descriptor Layout](docs/rfc/0002-minimal-hid-descriptor-layout.md)

Historical input remains in [engelbart-summary.md](engelbart-summary.md) and the review documents under `docs/`.
