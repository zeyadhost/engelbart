# ADR 0011: TinyUSB Integration Boundary

## Status

Accepted for Phase 1.6 architecture.

## Context

Engelbart targets TinyUSB in year one but should not make TinyUSB macros the user-facing authoring model. The descriptor engine still needs a clear boundary for generated TinyUSB callbacks, symbol ownership, HID instance mapping, and `tusb_config.h` alignment.

## Decision

Engelbart owns the TinyUSB descriptor callbacks for generated descriptors in year one:

- `tud_descriptor_device_cb`
- `tud_descriptor_configuration_cb`
- `tud_descriptor_string_cb`
- `tud_hid_descriptor_report_cb`

Engelbart generates static descriptor artifacts that these callbacks return or reference.

Application-owned callbacks remain user/application code, including HID data callbacks such as get-report, set-report, report-complete, and application event handling. Engelbart does not own user input scanning, report production, transfer scheduling, or business logic in year one.

Multiple HID interfaces map to TinyUSB HID instances in deterministic interface order:

- standalone mouse: HID instance `0`
- standalone keyboard: HID instance `0`
- composite mouse + keyboard: mouse HID instance `0`, keyboard HID instance `1`

`tusb_config.h` must align with generated descriptors:

- `CFG_TUD_HID` must equal the generated HID instance count.
- `CFG_TUD_ENDPOINT0_SIZE` must match generated `bMaxPacketSize0`.
- HID endpoint buffer sizing must support the generated HID endpoint packet size.
- Target and board macros must match the RP2040 / Pico SDK build.

Engelbart should validate detectable mismatches where practical and emit TinyUSB integration diagnostics for mismatches it can identify.

Generated symbols are classified as:

- public integration symbols: the TinyUSB callbacks listed above
- public inspection symbols: generated descriptor arrays and allocation tables intended for tests or user inspection
- private implementation symbols: helper constants, intermediate tables, and callback internals

"TinyUSB is an implementation detail" means TinyUSB is not the user-facing device model. It does not mean TinyUSB glue is hidden. Generated TinyUSB callbacks and symbol mappings must remain inspectable.

Engelbart does not provide a build-system wrapper, `engelbart build`, or `engelbart flash`.

## Rationale

Owning descriptor callbacks keeps generated descriptor bytes consistent with TinyUSB enumeration behavior. Leaving runtime HID behavior to application code preserves Engelbart's scope as a descriptor compiler rather than a USB application framework.

Deterministic HID instance mapping is required for composite devices and tests.

Explicit `tusb_config.h` alignment prevents a class of build-time and enumeration failures where descriptors and TinyUSB configuration disagree.

## Consequences

- The descriptor compiler must generate descriptor callback wiring as inspectable TinyUSB glue.
- Composite HID tests must verify instance order and report descriptor callback routing.
- TinyUSB configuration mismatch validation belongs in the validation engine.
- Runtime report behavior remains outside descriptor generation.

## Assumptions to verify during implementation

- The listed TinyUSB callback names and signatures match the TinyUSB version used with the Pico SDK.
- TinyUSB HID instance ordering follows descriptor/interface order for the generated configuration.
- Required `tusb_config.h` values are visible enough at compile time to validate or document clearly.
