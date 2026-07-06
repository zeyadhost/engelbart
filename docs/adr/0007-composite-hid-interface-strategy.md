# ADR 0007: Composite HID Interface Strategy

## Status

Accepted for Phase 1.5 architecture.

## Context

Composite HID mouse + keyboard can be represented in multiple ways. The two immediate options are separate HID interfaces, each with its own report descriptor, or one shared HID interface with multiple reports and report IDs.

The first descriptor engine needs the option that is easiest to inspect, validate, and test while still proving composite allocation.

## Decision

The first implementation uses separate HID interfaces by default for composite mouse + keyboard.

Mouse and keyboard each get their own HID interface and HID report descriptor. Interface numbering is deterministic. Endpoint allocation is deterministic.

Shared-interface HID with report IDs is deferred.

## Rationale

Separate interfaces make the generated output easier to inspect: one interface and one report descriptor per HID function.

This strategy directly exercises interface allocation, endpoint allocation, descriptor ordering, and TinyUSB callback generation without adding report-ID policy to the first descriptor engine.

It is also easier to test. Golden tests can compare the mouse descriptor, keyboard descriptor, interface table, and endpoint table independently.

## Tradeoffs

Separate interfaces may use more interface numbers and endpoints than a shared-interface design. Some devices prefer a compact single-interface HID model.

Shared-interface HID with report IDs is a valid design for later, but it introduces report ID allocation, combined report descriptors, and more complicated host-facing behavior.

The first implementation may not match every real-world composite HID device layout. That is acceptable because the project is optimizing first for correctness, determinism, and inspectability.

## Consequences

- Composite mouse + keyboard examples should describe separate HID interfaces.
- Report IDs are not required for the initial composite model.
- Interface allocation tests must prove deterministic mouse and keyboard interface ordering.
- Endpoint allocation tests must prove deterministic allocation for both HID interfaces.
- Shared-interface HID requires a later RFC or ADR.
