# ADR 0005: Compile-time String Policy

## Status

Accepted for Phase 1.5 architecture.

## Context

USB devices expose string descriptors for fields such as manufacturer, product, and optionally serial number. The minimal HID model needs manufacturer and product strings, but Engelbart must avoid runtime descriptor synthesis and hidden mutable state.

The first implementation also needs a C++20-friendly way to express strings in a static device description.

## Decision

Engelbart's first implementation should use a small `fixed_string`-style compile-time string wrapper for illustrative API design and implementation experiments.

Manufacturer and product strings are required for the minimal mouse, keyboard, and composite examples. They are part of the static device description. Empty manufacturer and product strings are not allowed in the minimal model.

Runtime string overlays are deferred for year one.

The exact implementation may change after compiler experiments with GCC ARM Embedded, but the architecture requires strings to remain compile-time, deterministic, and inspectable for the initial descriptor compiler.

ADR 0010 supersedes the unresolved year-one USB string descriptor details, including accepted character set, UTF-16LE encoding, string indexes, language ID, serial number policy, and maximum string length.

Illustrative, non-final shape:

```cpp
constexpr auto identity =
    engelbart::usb_device_identity{
        .vendor_id = engelbart::vid{0x1209},
        .product_id = engelbart::pid{0x0001},
        .manufacturer = engelbart::fixed_string{"Engelbart Example"},
        .product = engelbart::fixed_string{"Example HID Mouse"},
    };
```

## Rationale

A fixed-string wrapper keeps USB strings inside the typed compile-time device model. That supports deterministic descriptor bytes, byte-exact tests, static validation, and generated string descriptor inspection.

Requiring manufacturer and product strings in the first examples makes the minimal model realistic and avoids ambiguity around descriptor indexes.

Deferring runtime overlays avoids turning string descriptors into a back door for runtime descriptor construction before the static descriptor compiler is proven.

## Tradeoffs

C++20 non-type template parameter support for class types is useful but must be checked against the embedded GCC version used by the Pico SDK toolchain.

Fixed-string types can produce less readable compiler errors than ordinary string literals. Engelbart should compensate with validation IDs and focused diagnostics where practical.

Encoding USB string descriptors requires handling UTF-16LE output. The first policy may restrict accepted source strings to a documented ASCII subset before broader Unicode handling is designed.

Template-heavy string handling can contribute to compile-time cost and template bloat. The implementation should prefer the simplest representation that preserves compile-time validation and deterministic bytes.

Runtime overlays would be more flexible for serial numbers or user-configurable names, but they conflict with the year-one rule that descriptors are static by default.

## Consequences

- Manufacturer and product strings are required in the minimal HID examples.
- Empty manufacturer and product strings are validation failures.
- Runtime string overlays are not part of year-one scope.
- String descriptor bytes must be included in golden descriptor tests once implemented.
- Unicode policy remains intentionally conservative until a later RFC or ADR expands it.
