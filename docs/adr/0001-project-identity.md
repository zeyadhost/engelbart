# ADR 0001: Project Identity

## Status

Accepted for Phase 1 architecture.

## Context

The historical Engelbart documents describe a broad USB device framework and possible ecosystem. The architecture reviews narrow the durable opportunity to descriptor generation, composition, validation, and TinyUSB integration.

The project needs a precise identity before implementation begins.

## Decision

Engelbart is a compile-time USB device compiler for embedded C++.

Users describe a USB device with a high-level, strongly typed C++ API. Engelbart produces validated USB descriptors, HID report descriptors, endpoint layouts, interface numbering, composite device configuration, TinyUSB integration, and inspection artifacts.

Engelbart is compiler-like because its primary value is translating typed intent into deterministic generated artifacts. It is library-like in distribution and integration. It is not a platform in year one and should not own the whole development workflow.

## What Engelbart is

Engelbart is:

- a descriptor and composition compiler for embedded C++
- a validation layer for USB device descriptions
- a generator for TinyUSB-compatible integration on RP2040 / Pico SDK
- an inspectable source of generated USB and HID bytes

## Library, framework, platform, or compiler?

Engelbart should be described as a compiler first.

It will likely be consumed as a C++ library and CMake integration, but "library" alone understates the core behavior. The important action is compilation from device model to generated USB artifacts.

It is not a platform in year one. It should not own the IDE, build system, flashing workflow, desktop tools, or host inspection workflow.

## What it is in year one

In year one, Engelbart is the compile-time descriptor, composition, validation, and TinyUSB glue core for RP2040 / Raspberry Pi Pico WH using Pico SDK, TinyUSB, CMake, and GCC ARM Embedded.

The first useful class support is HID mouse, HID keyboard, and composite mouse + keyboard.

## Smallest useful version

The smallest useful version lets a user write a typed C++ description of a HID mouse or keyboard and get:

- byte-exact USB descriptors
- generated HID report descriptors
- deterministic endpoint allocation
- deterministic interface numbering
- TinyUSB glue for a Pico SDK project
- readable validation failures
- inspection output showing what was generated

## Capability that would invalidate the project if it failed

If Engelbart cannot reliably generate correct, deterministic, inspectable descriptors from typed device descriptions, the project fails.

Convenient wrappers alone are not enough. Without descriptor compilation and validation, Engelbart would duplicate existing TinyUSB wrapper libraries without a defensible reason to exist.

## Consequences

- Descriptor generation is the center of the architecture.
- Validation is a headline feature, not a helper.
- GUI, dashboard, CLI, Inspector, profiles, runtime reconfiguration, and multi-board expansion are deferred.
- TinyUSB remains the implementation layer, not the user-facing identity.
- Examples must avoid real-product impersonation.
