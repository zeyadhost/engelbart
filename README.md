# Engelbart Planning

Engelbart is a compile-time USB device compiler for embedded C++.

Users describe a USB device with a high-level, strongly typed C++ API. Engelbart generates USB descriptors, HID report descriptors, endpoint layouts, interface numbering, composite device configuration, TinyUSB integration, compile-time validation, and human-readable diagnostics.

Phase 1 is architecture only. The first target is RP2040 / Raspberry Pi Pico WH with Pico SDK and TinyUSB.

Start with:

- [Engineering principles](docs/principles.md)
- [Non-goals](docs/non-goals.md)
- [Roadmap](docs/roadmap.md)
- [ADR 0001: Project Identity](docs/adr/0001-project-identity.md)
- [RFC 0001: Minimal HID Device Model](docs/rfc/0001-minimal-hid-device-model.md)

Historical input remains in [engelbart-summary.md](engelbart-summary.md) and the review documents under `docs/`.
