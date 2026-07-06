# ADR 0015: Descriptor Byte Container and Endian Policy

## Status

Accepted for Phase 1.6 architecture.

## Context

Engelbart generates USB descriptor bytes. USB descriptors use little-endian encoding for multi-byte fields and explicit length fields. The red-team review identified that byte container, endian helpers, and length accounting policy were still implicit.

## Decision

Descriptor generation must use explicit byte emission helpers.

The descriptor engine must never rely on host struct layout, compiler packing, C++ object representation, or host endianness to generate USB descriptor bytes.

Multi-byte USB fields are emitted little-endian explicitly:

- 16-bit value `0x1234` emits `0x34, 0x12`
- 32-bit values are not expected in the minimal HID descriptors, but if used later they must also be emitted least-significant byte first

Length fields are calculated by the generator, not hand-maintained:

- `bLength` is the emitted byte count for that descriptor
- `wTotalLength` is the emitted byte count for the full configuration descriptor tree
- HID `wDescriptorLength` is the emitted byte count of the referenced HID report descriptor
- USB string descriptor `bLength` is `2 + 2 * code_unit_count`

Exact C++ container names are implementation details before 1.0. The public contract is deterministic byte output, not a specific container type.

Tests must verify emitted bytes and all length fields.

## Rationale

Explicit byte emission prevents accidental dependence on host compiler layout, padding, or endianness. It also makes byte-exact golden tests meaningful.

Calculated length fields reduce descriptor drift when report descriptors or composite layouts change.

Keeping container names private avoids freezing an API before the descriptor engine exists.

## Consequences

- Descriptor generator code should be organized around byte emission and length accounting.
- Golden tests must fail if any length field disagrees with emitted bytes.
- Inspection output should show both raw bytes and decoded length fields.
- Public examples should not promise a specific C++ descriptor container type before 1.0.
