# Scope Narrowing

Engelbart started with a broader set of ideas around USB device development, inspection, examples, and tooling. The current project intentionally narrowed that direction before implementation.

The accepted identity is:

> Engelbart is a compile-time USB device compiler for embedded C++.

The year-one target is RP2040 / Raspberry Pi Pico WH with Pico SDK, TinyUSB, CMake, GCC ARM Embedded, and C++20 where practical.

## What Was Narrowed

Early planning considered features that would have made Engelbart look like a larger product ecosystem: command-line build flows, dashboards, GUI tooling, host-side inspection tools, multiple board families, broad USB class support, profiles, and runtime reconfiguration.

Those ideas are not part of the current project scope. The core implementation is the descriptor/composition/validation layer for a small set of HID devices.

## Current Boundary

Year one is limited to:

- HID mouse
- boot-protocol-compatible HID keyboard
- composite mouse + keyboard using separate HID interfaces
- deterministic descriptor generation
- endpoint and interface allocation
- validation diagnostics
- inspection output
- TinyUSB integration for the generated descriptors

CDC, MIDI, gamepad, vendor class, multi-board support, runtime overlays, CLI workflows, dashboards, GUIs, and host-side inspectors are deferred or out of scope unless a later ADR explicitly changes that.

## Reason

The descriptor engine is the project risk. If Engelbart cannot generate byte-exact, inspectable, validated USB descriptors for a small HID model, broader tooling would not make the project useful.

The narrowed scope keeps Phase 2 focused on the capability that must work first: turning a static C++ device description into deterministic USB descriptor bytes and TinyUSB-facing artifacts.
