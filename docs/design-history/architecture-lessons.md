# Architecture Lessons

These lessons came from the planning and review work before descriptor-engine implementation.

## Descriptor Bytes Are The Product

Engelbart can have a pleasant C++ API, but the generated USB bytes are the compatibility surface. Tests and reviews should treat descriptor byte changes as meaningful, even before 1.0.

## Compile Time Is A Constraint, Not A Slogan

The project favors compile-time generation and validation, but embedded C++20 support has practical limits. The implementation should verify compiler behavior early, especially around class-type non-type template parameters and diagnostics.

## TinyUSB Should Be Contained

TinyUSB is the first implementation layer, not Engelbart's public identity. Generated TinyUSB glue must remain inspectable, but application code should not be forced to think of Engelbart as a TinyUSB wrapper.

## Target Limits Must Be Explicit

RP2040 / Pico WH is the first target. Endpoint limits, packet sizes, polling intervals, HID instance counts, and `tusb_config.h` expectations should be modeled directly instead of discovered accidentally through failed builds.

## Inspection Needs A Baseline

Human-readable inspection does not require a CLI or GUI. The minimum baseline is descriptor hex, decoded descriptor fields, HID report descriptor bytes, endpoint allocation, interface numbering, string descriptors, and TinyUSB callback mapping.

## Avoid Product Impersonation

Examples should use non-impersonating test identity values such as pid.codes-style VID `0x1209`. Engelbart is not a spoofing toolkit, anti-cheat tool, BadUSB framework, or USB attack framework.

## Public Docs Should Track Implemented Reality

Roadmaps, ADRs, RFCs, examples, and build instructions should distinguish implemented features from deferred decisions. Public documentation should not imply CLI, GUI, dashboard, inspector, multi-board, or runtime descriptor support exists.
