# ADR 0013: Minimum Inspection Baseline

## Status

Accepted for Phase 1.6 architecture.

## Context

Engelbart requires inspectable generated output. ADR 0008 deferred stable JSON and avoided CLI/GUI/Inspector scope, but the red-team review found that the minimum inspection output was still undefined.

## Decision

The year-one descriptor engine must provide a mandatory inspection baseline. The format is mandatory but not stable before 1.0.

The baseline must include:

- hex byte dump for every generated descriptor array
- decoded device descriptor field table
- decoded configuration descriptor field table
- decoded interface descriptor field table
- decoded HID descriptor field table
- decoded endpoint descriptor field table
- HID report descriptor byte dump
- endpoint allocation table
- interface numbering table
- string descriptor table
- TinyUSB callback and generated symbol map

JSON remains deferred unless a later ADR stabilizes it.

No CLI, GUI, dashboard, or host-side Inspector is required for this baseline. The baseline may be emitted by tests, examples, generated text artifacts, or documented build outputs.

## Rationale

Raw C++ static arrays are necessary but insufficient for the inspectability promise. Users and tests need a readable way to see descriptor fields, allocation decisions, and TinyUSB callback routing.

Keeping the format unstable before 1.0 allows the implementation to evolve without creating a premature external schema.

## Consequences

- Descriptor engine work must include inspection output, not only byte arrays.
- Golden tests may use the baseline for review, but byte-exact descriptor tests remain the primary compatibility checks.
- Tooling may not treat the baseline as a stable public schema before 1.0 unless a later ADR says so.
- JSON output remains out of scope.
