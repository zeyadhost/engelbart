# ADR 0009: Validation Diagnostics Policy

## Status

Accepted for Phase 1.5 architecture.

## Context

Validation is a headline feature for Engelbart. Some constraints can be checked with C++ compile-time mechanisms, but complete compiler diagnostics are not fully controllable and vary across compilers.

The project needs a policy for what fails at compile time, how diagnostics are identified, and how tests should verify them.

## Decision

Structural constraints should fail at compile time where practical.

Validation failures should include stable diagnostic IDs. Tests should assert diagnostic IDs and key message fragments, not entire compiler output.

Some validations may be easier to exercise through focused compile-failure test projects or generation test harnesses. Diagnostic quality is a product feature, but exact compiler formatting is not fully controllable.

## Rationale

Compile-time validation matches Engelbart's identity as a compile-time USB device compiler. It catches invalid device descriptions before firmware runs and before users debug enumeration failures on hardware.

Stable diagnostic IDs give tests, documentation, and users a durable handle even when compiler output formatting changes.

Testing key fragments instead of complete output avoids brittle tests tied to one compiler version, standard library implementation, or template instantiation style.

## Tradeoffs

Some readable diagnostics may require additional helper types, named validation layers, or carefully written `static_assert` messages. That can add implementation complexity.

Not every constraint will fit cleanly into a readable `static_assert`. Some checks may be clearer in focused generation tests or compile-failure fixtures.

Stable diagnostic IDs create a compatibility surface. Before 1.0 they may still change, but changes should be documented once IDs are public.

Compiler differences remain real. GCC ARM Embedded is the year-one toolchain target, but tests should not assume complete diagnostic text beyond IDs and key fragments.

## Consequences

- Validation rules should have stable IDs once exposed publicly.
- Compile-failure tests should assert IDs and key message fragments.
- Tests should avoid overfitting to full compiler error output.
- Structural descriptor constraints should be compile-time failures where practical.
- Diagnostic ID compatibility should be tracked in versioning policy.
