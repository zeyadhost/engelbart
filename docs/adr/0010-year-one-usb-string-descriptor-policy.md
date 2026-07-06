# ADR 0010: Year-one USB String Descriptor Policy

## Status

Accepted for Phase 1.6 architecture.

## Context

ADR 0005 chose a compile-time string direction but left byte-level string descriptor behavior open. The descriptor engine needs fixed rules for accepted input, string indexes, language ID descriptors, serial number handling, and UTF-16LE output before implementation begins.

## Decision

Year one accepts printable ASCII strings only for USB manufacturer and product strings.

Accepted characters are bytes `0x20` through `0x7E`. NUL, control characters, non-ASCII bytes, and empty strings are validation failures.

Accepted ASCII characters are encoded into USB string descriptors as UTF-16LE code units with a zero high byte. For example, ASCII `A` is emitted as `0x41, 0x00`.

The language ID descriptor is fixed to English (United States), `0x0409`, for year one.

String descriptor indexes are fixed for the minimal model:

- index `0`: language ID descriptor, containing only `0x0409`
- index `1`: manufacturer string
- index `2`: product string
- index `3` and above: unused in the first implementation

Serial number string descriptors are omitted in the first implementation. The USB device descriptor must set `iSerialNumber = 0`. Static serial number support requires a later ADR or RFC. Runtime serial overlays remain deferred.

Manufacturer and product strings are required and non-empty for Engelbart's minimal year-one model. This is an Engelbart policy for deterministic examples and stable string indexes, not a USB specification requirement.

The source string length limit for year one is 31 accepted ASCII characters per string. This produces a maximum string descriptor length of 64 bytes: `2 + 31 * 2`. Longer strings are validation failures.

`fixed_string` remains the likely first implementation mechanism for compile-time strings, but it is not yet a stable public API. RFC examples may use `engelbart::fixed_string` illustratively. The exact C++ syntax may change after GCC ARM Embedded experiments.

## Rationale

Printable ASCII avoids early Unicode normalization, encoding, and compiler source-encoding questions while still supporting useful examples.

Fixed indexes make descriptor bytes deterministic and keep golden tests simple. Omitting serial numbers avoids host caching and runtime identity questions until the core descriptor compiler is proven.

The 31-character limit keeps string descriptor length accounting simple and below the single-byte `bLength` limit by a wide margin.

## Consequences

- String descriptor bytes can be golden-tested exactly.
- Invalid string input has a clear validation surface.
- `iManufacturer = 1`, `iProduct = 2`, and `iSerialNumber = 0` are part of the year-one descriptor layout.
- Non-ASCII product names are intentionally unsupported until a later Unicode/string policy ADR.
- Static or runtime serial number support remains out of scope until explicitly added.

## Assumptions to verify during implementation

- GCC ARM Embedded can support the chosen compile-time string mechanism without excessive diagnostic noise or compile-time cost.
- The C++ source encoding used by supported examples maps accepted printable ASCII bytes directly.
