# ADR 0006: Initial Keyboard Model

## Status

Accepted for Phase 1.5 architecture.

## Context

The minimal HID RFC must choose a first keyboard shape before descriptor engine work begins. HID keyboard support can range from a simple boot-protocol keyboard to NKRO layouts, media keys, locale-aware key mapping, and multiple report collections.

Engelbart's first keyboard support should prove descriptor generation and validation without pulling in broad keymap policy.

## Decision

The first implementation supports a boot-protocol-compatible keyboard model only.

NKRO, media keys, locale-aware keyboard policy, and rich multi-report keyboard models are deferred. Any richer keyboard model requires a later RFC before implementation.

The initial keyboard model should generate a known, small HID report descriptor suitable for the standard boot keyboard shape.

## Rationale

Boot keyboard is the smallest useful keyboard model and is familiar to embedded USB developers. It is easier to validate, easier to inspect, and easier to test byte-for-byte.

Deferring NKRO and media keys avoids early complexity around report IDs, multiple reports, OS quirks, rollover policy, and composite HID behavior.

Deferring locale policy keeps Engelbart focused on USB descriptor compilation rather than keymap translation or text input semantics.

## Tradeoffs

Boot keyboard support is less capable than many real keyboard firmware projects need. Users who need NKRO, consumer control keys, or advanced layouts will have to wait for later RFCs.

A boot-only model may make early examples look conservative. That is acceptable because the goal is to prove deterministic descriptor generation and readable validation first.

Some host behavior differs between boot and report protocol. The initial model should document what it generates and avoid implying that it covers all keyboard use cases.

## Consequences

- The RFC examples should show boot keyboard as the only initial keyboard model.
- HID report descriptor golden tests should include the boot keyboard descriptor.
- NKRO, media keys, locale policy, and multiple keyboard reports remain out of scope for the first implementation.
- Validation should reject unsupported keyboard options rather than silently generating richer descriptors.
