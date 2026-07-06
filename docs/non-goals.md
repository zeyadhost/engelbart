# Engelbart Non-goals

Engelbart is intentionally narrow in year one: a compile-time USB device compiler for embedded C++ targeting RP2040 / Raspberry Pi Pico WH with Pico SDK and TinyUSB.

This document defines what Engelbart will not become. The purpose is practical scope control, not broad positioning.

## Spoofing toolkit

Engelbart is not a toolkit for impersonating existing USB products.

Why: spoofing-oriented examples create legal, ethical, and community trust problems. They also distort the API toward matching arbitrary existing devices instead of generating correct, inspectable descriptors for user-owned firmware.

## Anti-cheat tool

Engelbart is not an anti-cheat bypass tool or an input-device evasion framework.

Why: that use case would drive the project toward deception, device impersonation, and arms-race behavior instead of maintainable embedded USB development.

## USB attack framework

Engelbart is not a USB exploitation or attack framework.

Why: attack tooling has different incentives: stealth, payload delivery, unusual host behavior, and adversarial compatibility. Engelbart's priorities are deterministic output, validation, and readable diagnostics for legitimate device firmware.

## BadUSB framework

Engelbart is not a BadUSB framework.

Why: BadUSB-style automation is a security-abuse pattern, not a descriptor compiler goal. Engelbart may support HID keyboard descriptors, but examples and APIs should not be framed around unauthorized automation or impersonation.

## GUI-first application

Engelbart is not a GUI-first product.

Why: a GUI would become its own product surface, release process, design system, and support burden. The core problem is the gap between typed device intent and generated USB bytes.

## Build-system replacement

Engelbart is not a replacement for CMake, the Pico SDK build flow, or board flashing tools.

Why: wrapping build and flash workflows would make Engelbart responsible for unrelated toolchain failures. Year one should integrate with CMake rather than own the whole workflow.

## Runtime descriptor interpreter

Engelbart is not a runtime descriptor interpreter.

Why: building descriptor trees on-device in RAM conflicts with deterministic output, zero-cost abstractions, simple inspection, and small embedded targets. The core compiler should produce static descriptors.

## Electron desktop application

Engelbart is not an Electron desktop application.

Why: desktop application development is unrelated to the year-one embedded compiler. It would add a large technology stack before the descriptor model is proven.

## All-in-one embedded ecosystem

Engelbart is not an all-in-one embedded ecosystem.

Why: owning logging, configuration, flashing, dashboards, networking, project templates, and board support would dilute the descriptor compiler. Engelbart should remain composable with existing embedded tools.

## USB host framework

Engelbart is not a USB host framework.

Why: USB host stacks solve enumeration, hub management, device drivers, and host-side policy. Engelbart's year-one value is generating USB device descriptors and TinyUSB device integration.

## RTOS-specific framework

Engelbart is not tied to a specific RTOS and does not require one in year one.

Why: the initial target is Pico SDK on RP2040 / Pico WH. Introducing RTOS-specific architecture would make the smallest useful version harder to build, test, and explain.

## Dashboard, Studio, or Inspector commitment

Engelbart does not commit to a dashboard, Studio, or host-side Inspector in the core roadmap.

Why: those may be useful products later, but each depends on a stable descriptor model. Building them first would create a broad maintenance surface without proving the core compiler.

## Multi-board promise

Engelbart does not promise broad board support in year one.

Why: USB device behavior is shaped by controller limits, stack integration, memory layout, and build tooling. RP2040 / Pico WH support should be correct before additional boards are considered.
