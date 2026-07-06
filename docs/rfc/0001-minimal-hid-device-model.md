# RFC 0001: Minimal HID Device Model

## Status

Draft.

## Summary

This RFC describes the intended minimal Engelbart user model for HID mouse, HID keyboard, and composite mouse + keyboard devices on RP2040 / Raspberry Pi Pico WH.

The examples are illustrative and non-final. They are not implementation code and do not define exact API names.

Descriptor byte layout is pinned separately in [RFC 0002](0002-minimal-hid-descriptor-layout.md).

## Motivation

The smallest useful Engelbart version should let an embedded C++ developer describe a simple HID device without hand-writing USB descriptors, HID report descriptors, endpoint numbers, interface numbers, or TinyUSB descriptor callbacks.

The output must remain deterministic, inspectable, and validated.

## Goals

- Define a minimal user-facing shape for USB device identity.
- Define VID/PID policy for examples and tests.
- Support manufacturer and product strings.
- Support HID mouse.
- Support HID keyboard.
- Support composite mouse + keyboard.
- Generate USB descriptors.
- Generate HID report descriptors.
- Generate TinyUSB glue.
- Report validation failures clearly.
- Provide descriptor inspection output.

## Non-goals

- Final production API syntax.
- CDC, MIDI, gamepad, vendor class, or other USB classes.
- Multiple boards beyond RP2040 / Pico WH.
- Runtime descriptor reconfiguration.
- CLI, dashboard, GUI, host-side Inspector, or profiles.
- Real-product impersonation examples.

## Example user-facing API

The snippets in this section are illustrative and non-final.

### Device identity

Examples should use pid.codes-style test VID values such as `0x1209`. They must not impersonate real USB products.

```cpp
// Illustrative, non-final API.
constexpr auto identity =
    engelbart::usb_device_identity{
        .vendor_id = engelbart::vid{0x1209},
        .product_id = engelbart::pid{0x0001},
        .manufacturer = engelbart::fixed_string{"Engelbart Example"},
        .product = engelbart::fixed_string{"Example HID Mouse"},
    };
```

VID/PID policy:

- Examples use `0x1209` or another clearly documented test/development VID.
- Examples do not use real manufacturer names, real product strings, or known commercial VID/PID pairs.
- Project documentation should explain that production devices need a legitimate VID/PID assignment.

String policy:

- Manufacturer and product strings are required in the minimal examples.
- Empty manufacturer and product strings are validation failures.
- Strings are part of the static device description.
- Runtime string overlays are not part of year-one scope.
- Year-one string descriptor details are defined by [ADR 0010](../adr/0010-year-one-usb-string-descriptor-policy.md).

### HID mouse

```cpp
// Illustrative, non-final API.
constexpr auto device =
    engelbart::device(identity)
        .with(engelbart::hid::mouse{
            .buttons = 3,
            .x_axis = engelbart::hid::relative_axis<int8_t>{},
            .y_axis = engelbart::hid::relative_axis<int8_t>{},
            .wheel = engelbart::hid::relative_axis<int8_t>{},
        });

using generated = engelbart::compile<device, engelbart::target::pico_wh>;
```

Expected generated artifacts include the device descriptor, configuration descriptor, HID report descriptor, endpoint allocation, interface number, and TinyUSB descriptor glue.

### HID keyboard

```cpp
// Illustrative, non-final API.
constexpr auto keyboard_identity =
    engelbart::usb_device_identity{
        .vendor_id = engelbart::vid{0x1209},
        .product_id = engelbart::pid{0x0002},
        .manufacturer = engelbart::fixed_string{"Engelbart Example"},
        .product = engelbart::fixed_string{"Example HID Keyboard"},
    };

constexpr auto device =
    engelbart::device(keyboard_identity)
        .with(engelbart::hid::boot_keyboard{});

using generated = engelbart::compile<device, engelbart::target::pico_wh>;
```

The first keyboard model is intentionally conservative. It supports a boot-protocol-compatible keyboard only. NKRO, media keys, locale policy, and richer multi-report keyboard models are deferred.

The exact boot keyboard report descriptor layout is defined by [RFC 0002](0002-minimal-hid-descriptor-layout.md).

### Composite mouse + keyboard

```cpp
// Illustrative, non-final API.
constexpr auto composite_identity =
    engelbart::usb_device_identity{
        .vendor_id = engelbart::vid{0x1209},
        .product_id = engelbart::pid{0x0003},
        .manufacturer = engelbart::fixed_string{"Engelbart Example"},
        .product = engelbart::fixed_string{"Example Composite HID"},
    };

constexpr auto device =
    engelbart::device(composite_identity)
        .with(engelbart::hid::mouse{
            .buttons = 3,
            .x_axis = engelbart::hid::relative_axis<int8_t>{},
            .y_axis = engelbart::hid::relative_axis<int8_t>{},
            .wheel = engelbart::hid::relative_axis<int8_t>{},
        })
        .with(engelbart::hid::boot_keyboard{});

using generated = engelbart::compile<device, engelbart::target::pico_wh>;
```

The composite case is mandatory for the minimal model because it validates endpoint allocation, interface allocation, report descriptor separation, and TinyUSB callback generation.

The first composite model uses separate HID interfaces for mouse and keyboard. Shared-interface HID with report IDs is deferred.

RFC 0002 defines the initial no-report-ID descriptor layout. Future report-ID support must be explicit opt-in and must not silently replace the initial no-report-ID descriptors.

## Generated artifacts

For the minimal HID model, Engelbart should generate:

- USB device descriptor
- Configuration descriptor
- HID interface descriptors
- HID report descriptors
- Endpoint descriptors
- Interface numbering
- Endpoint allocation
- TinyUSB descriptor callbacks
- Any required target-specific constants for Pico SDK / TinyUSB
- Inspection artifacts for human review and tests

The first implementation should expose generated descriptors as C++ compile-time/static artifacts. Human-readable inspection dumps may be produced by tests or examples. Stable JSON output is deferred.

The year-one minimum inspection baseline is defined by [ADR 0013](../adr/0013-minimum-inspection-baseline.md). JSON remains deferred unless a later ADR stabilizes it.

## Validation model

Validation should happen as early as practical. Some failures should be compile-time failures; others may be emitted by generation tests or build-time tooling if C++ diagnostics are not readable enough.

Expected validation categories:

- invalid VID/PID values
- missing manufacturer or product string when required by policy
- endpoint budget exceeded
- unsupported HID field combination
- unsupported report size
- duplicate or conflicting interface assignments
- target unsupported for the requested device model
- generated descriptor length mismatch
- TinyUSB configuration mismatch

Initial diagnostic ID categories are defined by [ADR 0014](../adr/0014-diagnostic-id-taxonomy.md).

Validation messages should describe the user's device model, the violated constraint, and the target limit.

Example diagnostic wording, not final:

```text
EGB-ENDPOINT-001: endpoint budget exceeded for target pico_wh.
Requested 5 IN interrupt endpoints; the current RP2040 HID model supports 4.
```

Diagnostic IDs should be stable once exposed publicly. Tests should assert diagnostic IDs and key message fragments, not complete compiler output.

## Inspectability model

Users should be able to inspect:

- raw USB descriptor bytes
- decoded USB descriptor fields
- raw HID report descriptor bytes
- decoded HID report descriptor fields where practical
- endpoint allocation table
- interface numbering table
- generated TinyUSB glue

The inspection model should support byte-exact tests and human review. It should not require a GUI, dashboard, CLI, or host-side Inspector in year one.

TinyUSB integration ownership is defined by [ADR 0011](../adr/0011-tinyusb-integration-boundary.md). RP2040 endpoint and target limits are defined by [ADR 0012](../adr/0012-rp2040-target-limits-and-endpoint-policy.md). Descriptor byte emission and endian policy are defined by [ADR 0015](../adr/0015-descriptor-byte-container-and-endian-policy.md).

## Open questions

Resolved by ADRs:

- Compile-time string descriptor policy is resolved by [ADR 0005](../adr/0005-compile-time-string-policy.md).
- Initial keyboard scope is resolved by [ADR 0006](../adr/0006-initial-keyboard-model.md).
- Composite HID interface strategy is resolved by [ADR 0007](../adr/0007-composite-hid-interface-strategy.md).
- Generated artifact surfacing is resolved by [ADR 0008](../adr/0008-generated-artifact-surfacing.md).
- Validation diagnostic policy is resolved by [ADR 0009](../adr/0009-validation-diagnostics-policy.md).
- Year-one USB string descriptor details are resolved by [ADR 0010](../adr/0010-year-one-usb-string-descriptor-policy.md).
- TinyUSB integration boundary is resolved by [ADR 0011](../adr/0011-tinyusb-integration-boundary.md).
- RP2040 endpoint and target policy is resolved by [ADR 0012](../adr/0012-rp2040-target-limits-and-endpoint-policy.md).
- Minimum inspection baseline is resolved by [ADR 0013](../adr/0013-minimum-inspection-baseline.md).
- Diagnostic ID taxonomy is resolved by [ADR 0014](../adr/0014-diagnostic-id-taxonomy.md).
- Descriptor byte container and endian policy is resolved by [ADR 0015](../adr/0015-descriptor-byte-container-and-endian-policy.md).
- Minimal HID descriptor layout is resolved by [RFC 0002](0002-minimal-hid-descriptor-layout.md).

Still open:

- Exact C++ API names remain illustrative and non-final until implementation experiments validate the syntax on GCC ARM Embedded.
- The precise stable inspection artifact format remains deferred; ADR 0013 defines a mandatory but unstable baseline, and JSON is explicitly not stable in year one unless a later ADR says otherwise.

## Acceptance criteria

- A user can describe an illustrative HID mouse without raw descriptor bytes.
- A user can describe an illustrative HID keyboard without raw descriptor bytes.
- A user can describe an illustrative composite mouse + keyboard device.
- Generated USB descriptors are deterministic and byte-exact testable.
- Generated HID report descriptors are deterministic and byte-exact testable.
- Endpoint allocation and interface numbering are deterministic and inspectable.
- TinyUSB glue is generated for RP2040 / Pico SDK.
- Validation failures are readable and actionable.
- Descriptor bytes follow RFC 0002.
- No example impersonates a real USB product.
