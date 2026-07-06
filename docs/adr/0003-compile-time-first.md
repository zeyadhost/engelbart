# ADR 0003: Compile-time-first Architecture

## Status

Accepted for Phase 1 architecture.

## Context

Embedded USB descriptors are structured byte data with strict host compatibility expectations. They are also highly testable: a device description should map to known descriptor bytes, endpoint allocation, interface numbering, and HID report descriptors.

Engelbart must choose between runtime descriptor construction and compile-time generation.

## Decision

Engelbart is compile-time first.

Descriptors should be generated at compile time whenever practical. Generated descriptors should not require heap allocation. Output should be deterministic. Validation should happen as early as possible, preferably at compile time for constraints expressible in C++20.

Runtime descriptor tree construction is rejected for the core design.

## Design expectations

- Generated USB descriptors are static data by default.
- HID report descriptors are generated from typed declarations where practical.
- Endpoint and interface allocation happen deterministically.
- Validation failures should be readable and actionable.
- Runtime state is for device behavior, transfers, and application logic, not descriptor synthesis.
- Runtime overlays, if ever supported, must be tightly bounded and documented.

## Runtime overlays

Some future features may justify limited runtime overlays, such as selecting a string table or serial number from nonvolatile storage.

These overlays must not become arbitrary runtime descriptor graph construction. They should preserve deterministic structure, fixed bounds, and inspectable output.

## Tradeoffs

Compile-time C++ has limits:

- C++20 constexpr support varies across compiler versions.
- Compile-time diagnostics can be awkward compared with normal runtime error messages.
- Template-heavy APIs can increase build times.
- Some validations may be easier to express in generated test tools than in pure `static_assert`.
- Debugging compile-time code can be harder than debugging runtime code.

These costs are accepted because deterministic, byte-exact, zero-cost output is central to Engelbart's identity.

## Consequences

- The descriptor compiler should be designed as a mostly pure transformation from typed model to bytes.
- Tests should compare generated descriptors byte-for-byte.
- APIs should avoid hidden runtime registration or constructor side effects.
- Features requiring heap-backed runtime descriptor synthesis are out of scope for the core.
