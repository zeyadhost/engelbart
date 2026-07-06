# ADR 0012: RP2040 Target Limits and Endpoint Policy

## Status

Accepted for Phase 1.6 architecture.

## Context

The year-one descriptor engine includes endpoint and interface allocation for RP2040 / Pico WH. The red-team review identified that endpoint limits and allocation rules were not explicit enough to implement or test.

Exact hardware and TinyUSB limits must be verified during implementation. Until then, the architecture should use a conservative year-one target model rather than accidental implementation choices.

## Decision

The year-one RP2040 / Pico WH target model is conservative:

- endpoint 0 is reserved for control transfers
- generated HID functions use interrupt IN endpoints only
- HID OUT endpoints are unsupported in year one
- the compiler model supports at most four generated HID interrupt IN endpoints
- the minimal descriptor layouts use at most two generated HID interrupt IN endpoints
- endpoint addresses are allocated deterministically in HID interface order

Endpoint address allocation:

- first HID IN endpoint: `0x81`
- second HID IN endpoint: `0x82`
- third HID IN endpoint, reserved for future year-one-compatible HID expansion only: `0x83`
- fourth HID IN endpoint, reserved for future year-one-compatible HID expansion only: `0x84`

Endpoint number reuse by direction is not used in the initial policy. Each generated HID endpoint gets a unique endpoint number. OUT endpoints are rejected as unsupported.

Endpoint defaults:

- endpoint type: interrupt
- direction: IN
- `wMaxPacketSize`: `8`
- `bInterval`: `10`

Target buffer assumptions:

- descriptors are static data
- no heap allocation is required for descriptors
- TinyUSB endpoint buffers must be configured to support the generated `wMaxPacketSize`
- no isochronous, bulk, or HID OUT endpoint buffers are allocated by Engelbart in year one

The compiler target model should represent limits as data, not as scattered constants:

- max generated HID IN endpoints: `4`
- max generated HID OUT endpoints: `0`
- endpoint 0 packet size: `64`
- HID interrupt packet size default: `8`
- HID interrupt interval default: `10`
- supported speed: full speed only
- supported configurations: `1`

Endpoint exhaustion diagnostics should name the target, requested count, supported count, and unsupported endpoint direction if relevant.

Example wording:

```text
EGB-ENDPOINT-001: endpoint budget exceeded for target pico_wh.
Requested 5 HID IN interrupt endpoints; this year-one RP2040 model supports 4.
```

## Rationale

A conservative endpoint model is easier to validate and test. It avoids encoding unverified assumptions about the full RP2040/TinyUSB endpoint matrix into public behavior.

The minimal HID devices need only one or two IN interrupt endpoints, so a four-IN-endpoint compiler limit is enough for year-one HID work while leaving room for focused tests of exhaustion behavior.

## Consequences

- The minimal mouse uses endpoint `0x81`.
- The minimal keyboard uses endpoint `0x81`.
- The composite mouse + keyboard uses mouse endpoint `0x81` and keyboard endpoint `0x82`.
- HID OUT endpoints are validation failures.
- Endpoint allocation tests must verify addresses, packet size, interval, and exhaustion diagnostics.
- If implementation verifies different safe RP2040/TinyUSB limits, changing this policy requires an ADR update and golden test review.

## Assumptions to verify during implementation

- RP2040 / TinyUSB can support the conservative endpoint set defined here.
- `CFG_TUD_HID` and TinyUSB endpoint buffer settings can be made consistent with generated HID instance count and packet size.
- Endpoint address allocation `0x81` through `0x84` is compatible with TinyUSB's RP2040 device controller backend.
