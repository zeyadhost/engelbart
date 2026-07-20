# Engelbart — AI agent instructions

Canonical, machine-readable instructions for AI coding agents working in this
repository. Human contributors follow `CONTRIBUTING.md`. Where this file
summarizes a larger document, it links to the authoritative source instead of
duplicating it.

## Project overview

Engelbart is a compile-time USB device compiler for embedded C++ (header-only,
C++20). A user describes a USB device with a small typed API; Engelbart lowers
that description to byte-exact USB descriptors, HID report descriptors, endpoint
and interface allocation, validation diagnostics, inspection output, and
TinyUSB-facing callback glue, all at compile time. Year-one target is Raspberry
Pi Pico WH / RP2040 with the Pico SDK, TinyUSB, and GCC ARM Embedded.

## Project goals

- Generate byte-exact, inspectable USB descriptors at compile time.
- Keep generated artifacts deterministic and zero-cost.
- Support RP2040 / Pico WH via Pico SDK and TinyUSB as the year-one target.
- Validate device descriptions early with stable, testable diagnostic IDs.

See `README.md` for scope and status.

## Non-goals

Explicitly out of scope: CLI, GUI, dashboard, or Inspector; CDC, MIDI, gamepad,
or vendor class; multi-board support; runtime descriptor synthesis or
reconfiguration; and any BadUSB, impersonation, or anti-cheat framing. Full,
binding list: `docs/non-goals.md`.

## Target audience

Maintainers and contributors building or extending a typed USB descriptor
compiler for embedded C++. This is not end-user application code.

## Compiler architecture summary

Engelbart is a pure compile-time compiler, not a runtime library of USB
constants. Pipeline: typed model -> validation -> canonical form -> IR ->
allocated IR -> descriptors -> bytes -> backend view. Passes (validation,
allocation, emission, backend) are distinct. TinyUSB is a backend boundary, not
the authoring model. Generated descriptors are static `constexpr` data; no
runtime descriptor trees and no hidden global state. Full spec:
`docs/COMPILER_PIPELINE.md`. The shipped code is the v0.1.x three-layout proof;
the IR pipeline is the documented target, not yet fully implemented.

## Repository layout

- `include/engelbart/` — header-only core library
- `examples/` — host examples and the Pico WH smoke firmware
- `tests/` — host tests, golden descriptor fixtures, compile-failure tests
- `docs/` — ADRs, RFCs, design history, principles, non-goals, roadmap, pipeline
- `CMakeLists.txt`, `README.md`, `CONTRIBUTING.md`, `LICENSE`, `CHANGELOG.md`
- `.internal/` — private engineering workspace (see the split below)

## Public / private repository split

This public repository is the source of truth for shipped code and binding docs;
it must build, test, and release with no access to the private workspace. The
private workspace is nested at `.internal/` as a separate git repository. It is
never a build input, never a submodule, and never read by CMake or public code.
Promotion of finished work is a deliberate public pull request. Maintainers keep
the strict split in `.internal/BOUNDARY.md`. Public work must stand alone
without private access.

## Mandatory reading order

Load before non-trivial work:

1. `README.md`
2. `docs/principles.md`
3. `docs/non-goals.md`
4. Relevant `docs/adr/` entries and `docs/COMPILER_PIPELINE.md`
5. `docs/rfc/0002-minimal-hid-descriptor-layout.md` when descriptor bytes change
6. The headers, tests, and examples actually touched
7. `git log` / `git diff` when behavior or process matters
8. `CONTRIBUTING.md` for contributor rules

## Coding standards

- C++20, header-only; match the surrounding style (includes, naming, `constexpr`
  patterns).
- Touch the minimum surface that achieves the goal.
- Keep passes separated: validation, allocation, emission, and backend are
  distinct.
- Update golden descriptors deliberately with a reason; never as drive-by noise.
- Prefer plain C++ over clever template exhibitions unless the design requires
  it.
- Comments explain non-obvious invariants, not obvious lines.
- Examples must use the pid.codes test VID `0x1209`; never impersonate real USB
  products.
- Reject generic AI filler: one-use abstractions, `shared_ptr` webs in
  `constexpr` code, exception-heavy emission paths, comment banners, emoji,
  giant unified headers, and tests that only assert `true`.

## Review standards

Before claiming done, answer: what user-visible behavior changed? what bytes or
diagnostics changed? what remains unfinished? which public docs need updates?
Produce a PR-ready change summary; do not bury intent only in chat.

## Testing expectations

The host loop is the primary gate:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Tests cover descriptor generation, byte-exact goldens, HID reports, string
encoding, endpoint and interface allocation, validation failures and diagnostic
IDs, TinyUSB callback routing, and inspection output. Pico hardware enumeration
is manual or CI-hardware-dependent; the smoke firmware is not a full lab. Update
goldens when bytes change, with a reason.

## Documentation expectations

- Descriptor bytes change -> goldens plus release note; RFC if the layout law
  changes.
- Validation ID change -> tests plus versioning note.
- Compiler stage boundary change -> public pipeline or ADR docs.
- Public API behavior change -> `README.md` / examples if user-facing.
- Process-only change -> private workflow docs.

Do not write end-user marketing docs. Keep docs in sync with merged behavior;
avoid speculative documentation ahead of implementation.

## Semantic commit expectations

Commits are semantic, atomic, and review-friendly.

Header form:

```text
<type>[optional scope][optional !]: <imperative summary>
```

Types: `feat`, `fix`, `docs`, `test`, `refactor`, `perf`, `build`, `ci`,
`chore`, `revert`. Scope is a short noun (`ir`, `hid`, `alloc`, `emit`,
`tinyusb`, `tests`, `pico`, `setup`, `pipeline`). The summary is present-tense
imperative with no trailing period. The body explains non-obvious why. Mark
breaking changes with `!` and a `BREAKING CHANGE` footer. Descriptor byte changes
are compatibility-relevant even pre-1.0. One logical change per commit; public
commits never include `.internal/` contents. Full standard lives in
`.internal/prompts/AI_PROMPTING.md` (maintainer workspace).

## Things an AI must never do

- Redesign the public API, compiler internals, or architecture unless explicitly
  asked and approved.
- Add runtime descriptor builders, descriptor trees, or hidden global
  registration.
- Splice harness-only USB interfaces (for example the Pico reset interface) into
  core emission.
- Make CMake or code read from `.internal/`.
- Add features from `docs/non-goals.md` (CLI, GUI, CDC, MIDI, gamepad, vendor
  class, multi-board, runtime descriptor reconfiguration or interpretation).
- Write examples that impersonate real USB products.
- Claim tests or hardware results you did not run.

## Implementation philosophy

Prefer small, reviewable diffs over heroic rewrites. The repository (git
history, ADRs, RFCs, code) owns truth over chat memory. Public binding docs win
over private notes. No fake completion: if unverified, say so. Derived from
`.internal/prompts/AI_PROMPTING.md` §7.

## Documentation philosophy

Documentation records decisions that shipped code must obey or explains behavior
that already exists. Implementation drives documentation, not the reverse. Avoid
speculative docs. Private design docs must not outrank public ADRs.

## Pull request philosophy

One story per PR when possible. Standalone: understandable without private
context. Honest: no claims of unfinished behavior. Tested: goldens and tests
updated when bytes or validation change. The public PR must not require private
access. Keep CI green.

## Definition of done

- Goal, non-goals, and definition of done stated before code.
- Only the intended surface was touched; passes remain separated.
- Public docs, ADRs, or RFCs updated if binding behavior changed.
- Goldens and tests updated and the host test loop passes (state what was not
  run).
- No private files staged in the public repository.
- A semantic commit message is prepared per the commit standard.
- A change summary suitable for a PR description is produced.
