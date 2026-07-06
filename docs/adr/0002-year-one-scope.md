# ADR 0002: Year-one Scope

## Status

Accepted for Phase 1 architecture.

## Context

Engelbart needs a narrow year-one scope that proves the core compiler without becoming a multi-product ecosystem.

The target audience is an intermediate embedded developer comfortable with CMake, C++, Pico SDK, TinyUSB concepts, and embedded development.

## Decision

Year one targets RP2040 / Raspberry Pi Pico WH only, using Pico SDK, TinyUSB, CMake, and GCC ARM Embedded.

The initial USB class surface is HID mouse, HID keyboard, and composite mouse + keyboard.

## In scope

Year-one implementation planning includes:

- RP2040 / Raspberry Pi Pico WH only
- Pico SDK
- TinyUSB
- CMake integration
- GCC ARM Embedded
- HID mouse
- HID keyboard
- Composite HID mouse + keyboard
- Descriptor compiler
- Endpoint allocator
- Interface allocator
- HID report descriptor generation
- Validation engine
- Descriptor inspection output
- Tests and CI from the beginning

## Explicitly deferred

The following are out of year-one scope:

- CDC
- MIDI
- Gamepad
- Vendor class
- Multiple boards
- CLI
- Dashboard
- GUI
- Host-side Inspector
- Profiles
- Runtime reconfiguration

## Rationale

HID mouse and keyboard are small enough to implement and test, but composite mouse + keyboard still exercises the hard parts: multiple interfaces, report descriptors, endpoint allocation, and TinyUSB integration.

RP2040 / Pico WH keeps hardware, toolchain, and USB controller assumptions bounded while the descriptor model is still changing.

## Consequences

- The public roadmap should not promise dashboards, Studio, Inspector, or broad board support.
- Additional USB classes must wait until the descriptor compiler and validation model are proven.
- Tests can focus on pure descriptor generation and byte-exact output early.
- TinyUSB-specific glue may exist, but the user-facing model should remain USB-centered.
