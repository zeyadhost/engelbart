# ADR 0014: Diagnostic ID Taxonomy

## Status

Accepted for Phase 1.6 architecture.

## Context

ADR 0009 requires stable diagnostic IDs once exposed, but the initial ID namespace was not defined. Phase 2 needs a small taxonomy before validation rules and compile-failure tests multiply.

## Decision

Engelbart diagnostic IDs use this format:

```text
EGB-<CATEGORY>-NNN
```

`CATEGORY` is uppercase ASCII. `NNN` is a three-digit decimal number starting at `001` within each category.

Initial categories:

| Category | Example | Use |
|---|---|---|
| `STR` | `EGB-STR-001` | USB string descriptor validation and encoding. |
| `ID` | `EGB-ID-001` | VID, PID, device identity, and descriptor identity fields. |
| `ENDPOINT` | `EGB-ENDPOINT-001` | Endpoint allocation, endpoint limits, endpoint direction/type policy. |
| `INTERFACE` | `EGB-INTERFACE-001` | Interface numbering, interface count, duplicate assignments. |
| `HID` | `EGB-HID-001` | HID report model, unsupported HID fields, report descriptor policy. |
| `DESC` | `EGB-DESC-001` | Descriptor length, descriptor ordering, byte accounting. |
| `TARGET` | `EGB-TARGET-001` | Target support and target resource limits. |
| `TINYUSB` | `EGB-TINYUSB-001` | TinyUSB callback and configuration integration. |
| `UNSUPPORTED` | `EGB-UNSUPPORTED-001` | Explicitly unsupported feature requests. |

Diagnostic tests should assert IDs and key message fragments, not full compiler output.

IDs may change before 1.0. Once an ID appears in tests, examples, or public docs, changes must be documented. After 1.0, public diagnostic IDs are compatibility-relevant.

## Rationale

A category namespace keeps diagnostics searchable and testable without freezing exact compiler wording. It also prevents early endpoint or string IDs from becoming a disorganized list.

## Consequences

- New validation failures should choose one of the initial categories unless a new category is justified.
- Compile-failure tests should prefer ID checks.
- Release notes should call out public diagnostic ID changes once IDs are used outside local implementation tests.
