# Engelbart Roadmap

This roadmap is intentionally narrow. Engelbart is a compile-time USB device compiler for embedded C++, targeting RP2040 / Raspberry Pi Pico WH first with Pico SDK and TinyUSB.

## Phase 0: Planning

Historical planning and review.

Outcomes:

- project idea captured
- scope risks identified
- narrowed direction established
- questions collected before implementation

## Phase 1: Architecture

Outcomes:

- engineering principles
- non-goals
- ADRs for identity, year-one scope, compile-time-first design, and inspectability
- minimal HID device model RFC
- testing strategy
- versioning expectations
- focused roadmap

No production framework implementation belongs in this phase.

## Phase 1.5: Resolve Open Questions

Resolve implementation-blocking architecture questions before descriptor engine work begins.

Outcomes:

- compile-time string policy
- initial keyboard model
- composite HID interface strategy
- generated artifact surfacing policy
- validation diagnostics policy

No production framework implementation belongs in this phase.

## Phase 1.6: Resolve Red-team Blockers

Resolve byte-level architecture blockers identified by the Phase 1.5 red-team review.

Outcomes:

- year-one USB string descriptor policy
- minimal HID descriptor layout
- TinyUSB integration boundary
- RP2040 target limits and endpoint policy
- minimum inspection baseline
- diagnostic ID taxonomy
- descriptor byte container and endian policy

No production framework implementation belongs in this phase.

## Phase 2: Descriptor Engine

Build the core compiler model.

Target outcomes:

- typed device model
- USB descriptor generation
- deterministic byte output
- endpoint allocator
- interface allocator
- validation engine
- descriptor inspection artifacts
- byte-exact tests and CI

Scope remains RP2040 / Pico WH, Pico SDK, TinyUSB, CMake, and GCC ARM Embedded.

## Phase 3: Minimal HID Devices

Prove the descriptor engine with minimal HID devices.

Target outcomes:

- HID mouse
- HID keyboard
- composite mouse + keyboard
- HID report descriptor generation
- generated TinyUSB glue
- Pico SDK / TinyUSB build smoke tests
- examples that use non-impersonating test identities

## Phase 4: Additional USB Classes

Consider additional USB classes only after the descriptor engine and minimal HID devices are stable.

Possible candidates:

- CDC
- MIDI
- gamepad
- vendor class

These are not year-one commitments. Each class should require an RFC before implementation.

## Phase 5: Expansion

Consider expansion only after real use of the core compiler.

Possible areas:

- additional boards
- richer inspection artifact formats
- more HID models
- optional tooling built around stable generated artifacts

Dashboard, GUI, Studio, host-side Inspector, build-system replacement, runtime descriptor interpreter, and broad embedded ecosystem features are not committed roadmap items.
