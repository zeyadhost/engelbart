# RFC 0002: Minimal HID Descriptor Layout

## Status

Accepted for Phase 1.6 architecture.

## Summary

This RFC pins the year-one USB and HID descriptor layout for:

- standalone HID mouse
- standalone boot keyboard
- composite mouse + keyboard

The purpose is to give Phase 2 descriptor-engine work byte-level targets for golden tests. This is not implementation code.

## Shared USB Device Descriptor Policy

All minimal HID examples use one USB configuration.

| Field | Value | Notes |
|---|---:|---|
| `bLength` | `18` | Device descriptor length. |
| `bDescriptorType` | `0x01` | Device descriptor. |
| `bcdUSB` | `0x0200` | USB 2.0 full-speed device. |
| `bDeviceClass` | `0x00` | Class specified per interface. |
| `bDeviceSubClass` | `0x00` | Class specified per interface. |
| `bDeviceProtocol` | `0x00` | Class specified per interface. |
| `bMaxPacketSize0` | `64` | Endpoint 0 max packet size. Assumption to verify with TinyUSB/Pico SDK defaults. |
| `idVendor` | user supplied | Examples use `0x1209`. |
| `idProduct` | user supplied | Examples use non-impersonating test values. |
| `bcdDevice` | `0x0100` | Initial generated device release number. |
| `iManufacturer` | `1` | ADR 0010. |
| `iProduct` | `2` | ADR 0010. |
| `iSerialNumber` | `0` | Serial omitted in first implementation. |
| `bNumConfigurations` | `1` | Single configuration only. |

## Shared Configuration Descriptor Policy

| Field | Value | Notes |
|---|---:|---|
| `bLength` | `9` | Configuration descriptor length. |
| `bDescriptorType` | `0x02` | Configuration descriptor. |
| `wTotalLength` | layout-specific | Includes configuration, interface, HID, and endpoint descriptors. |
| `bNumInterfaces` | layout-specific | `1` for standalone, `2` for composite. |
| `bConfigurationValue` | `1` | Single configuration. |
| `iConfiguration` | `0` | No configuration string. |
| `bmAttributes` | `0x80` | Bus-powered. |
| `bMaxPower` | `50` | 100 mA. |

## Shared HID Descriptor Policy

| Field | Value | Notes |
|---|---:|---|
| `bLength` | `9` | HID descriptor length. |
| `bDescriptorType` | `0x21` | HID descriptor. |
| `bcdHID` | `0x0111` | HID 1.11. |
| `bCountryCode` | `0` | Not localized. |
| `bNumDescriptors` | `1` | One report descriptor. |
| `bDescriptorType` | `0x22` | Report descriptor. |
| `wDescriptorLength` | report-specific | Mouse `52`, keyboard `63`. |

## Shared Endpoint Policy

All minimal HID functions use one interrupt IN endpoint and no OUT endpoint.

| Field | Value | Notes |
|---|---:|---|
| `bLength` | `7` | Endpoint descriptor length. |
| `bDescriptorType` | `0x05` | Endpoint descriptor. |
| `bEndpointAddress` | allocation-specific | Standalone `0x81`; composite `0x81`, `0x82`. |
| `bmAttributes` | `0x03` | Interrupt endpoint. |
| `wMaxPacketSize` | `8` | Conservative default for mouse and keyboard. |
| `bInterval` | `10` | 10 ms polling interval. |

Endpoint packet size and interval are pinned for year one. Changes are descriptor byte changes and must update golden tests.

## No-report-ID Policy

The first implementation emits no HID report IDs.

Standalone mouse, standalone keyboard, and composite separate-interface HID all use report descriptors without `Report ID` items. Future report-ID support must be explicit opt-in and must not silently replace these initial no-report-ID descriptors.

## Standalone HID Mouse

The minimal mouse is a report-protocol HID mouse, not a boot-protocol mouse.

Rationale: the year-one mouse includes X, Y, wheel, and three buttons. Marking it as a boot mouse would create ambiguity around boot-protocol reports. A later RFC may add a separate boot-mouse model.

### Descriptor Order

1. Device descriptor
2. Configuration descriptor
3. Interface descriptor, mouse HID
4. HID descriptor, mouse report descriptor reference
5. Endpoint descriptor, mouse interrupt IN
6. String descriptor 0, language ID
7. String descriptor 1, manufacturer
8. String descriptor 2, product

### Interface Descriptor

| Field | Value | Notes |
|---|---:|---|
| `bLength` | `9` | Interface descriptor length. |
| `bDescriptorType` | `0x04` | Interface descriptor. |
| `bInterfaceNumber` | `0` | First and only interface. |
| `bAlternateSetting` | `0` | No alternate setting. |
| `bNumEndpoints` | `1` | One interrupt IN endpoint. |
| `bInterfaceClass` | `0x03` | HID. |
| `bInterfaceSubClass` | `0x00` | No boot subclass. |
| `bInterfaceProtocol` | `0x00` | No boot protocol. |
| `iInterface` | `0` | No interface string. |

### Endpoint Descriptor

Mouse endpoint address is `0x81`.

### Configuration Lengths

| Field | Value |
|---|---:|
| `wTotalLength` | `34` |
| `bNumInterfaces` | `1` |

Calculation: `9 configuration + 9 interface + 9 HID + 7 endpoint = 34`.

### Mouse HID Report Descriptor

Length: `52` bytes.

| Bytes | Meaning |
|---|---|
| `05 01` | Usage Page, Generic Desktop |
| `09 02` | Usage, Mouse |
| `A1 01` | Collection, Application |
| `09 01` | Usage, Pointer |
| `A1 00` | Collection, Physical |
| `05 09` | Usage Page, Button |
| `19 01` | Usage Minimum, Button 1 |
| `29 03` | Usage Maximum, Button 3 |
| `15 00` | Logical Minimum, 0 |
| `25 01` | Logical Maximum, 1 |
| `95 03` | Report Count, 3 |
| `75 01` | Report Size, 1 |
| `81 02` | Input, Data Variable Absolute |
| `95 01` | Report Count, 1 |
| `75 05` | Report Size, 5 |
| `81 03` | Input, Constant Variable Absolute |
| `05 01` | Usage Page, Generic Desktop |
| `09 30` | Usage, X |
| `09 31` | Usage, Y |
| `09 38` | Usage, Wheel |
| `15 81` | Logical Minimum, -127 |
| `25 7F` | Logical Maximum, 127 |
| `75 08` | Report Size, 8 |
| `95 03` | Report Count, 3 |
| `81 06` | Input, Data Variable Relative |
| `C0` | End Collection |
| `C0` | End Collection |

Hex:

```text
05 01 09 02 A1 01 09 01 A1 00 05 09 19 01 29 03
15 00 25 01 95 03 75 01 81 02 95 01 75 05 81 03
05 01 09 30 09 31 09 38 15 81 25 7F 75 08 95 03
81 06 C0 C0
```

## Standalone Boot Keyboard

The minimal keyboard is boot-protocol-compatible.

### Descriptor Order

1. Device descriptor
2. Configuration descriptor
3. Interface descriptor, keyboard HID
4. HID descriptor, keyboard report descriptor reference
5. Endpoint descriptor, keyboard interrupt IN
6. String descriptor 0, language ID
7. String descriptor 1, manufacturer
8. String descriptor 2, product

### Interface Descriptor

| Field | Value | Notes |
|---|---:|---|
| `bLength` | `9` | Interface descriptor length. |
| `bDescriptorType` | `0x04` | Interface descriptor. |
| `bInterfaceNumber` | `0` | First and only interface. |
| `bAlternateSetting` | `0` | No alternate setting. |
| `bNumEndpoints` | `1` | One interrupt IN endpoint. |
| `bInterfaceClass` | `0x03` | HID. |
| `bInterfaceSubClass` | `0x01` | Boot interface subclass. |
| `bInterfaceProtocol` | `0x01` | Keyboard. |
| `iInterface` | `0` | No interface string. |

### Endpoint Descriptor

Keyboard endpoint address is `0x81`.

### Configuration Lengths

| Field | Value |
|---|---:|
| `wTotalLength` | `34` |
| `bNumInterfaces` | `1` |

Calculation: `9 configuration + 9 interface + 9 HID + 7 endpoint = 34`.

### Keyboard HID Report Descriptor

Length: `63` bytes.

| Bytes | Meaning |
|---|---|
| `05 01` | Usage Page, Generic Desktop |
| `09 06` | Usage, Keyboard |
| `A1 01` | Collection, Application |
| `05 07` | Usage Page, Keyboard/Keypad |
| `19 E0` | Usage Minimum, Keyboard LeftControl |
| `29 E7` | Usage Maximum, Keyboard Right GUI |
| `15 00` | Logical Minimum, 0 |
| `25 01` | Logical Maximum, 1 |
| `75 01` | Report Size, 1 |
| `95 08` | Report Count, 8 |
| `81 02` | Input, modifier byte |
| `95 01` | Report Count, 1 |
| `75 08` | Report Size, 8 |
| `81 03` | Input, reserved byte |
| `95 05` | Report Count, 5 |
| `75 01` | Report Size, 1 |
| `05 08` | Usage Page, LEDs |
| `19 01` | Usage Minimum, Num Lock |
| `29 05` | Usage Maximum, Kana |
| `91 02` | Output, LED bits |
| `95 01` | Report Count, 1 |
| `75 03` | Report Size, 3 |
| `91 03` | Output, LED padding |
| `95 06` | Report Count, 6 |
| `75 08` | Report Size, 8 |
| `15 00` | Logical Minimum, 0 |
| `25 65` | Logical Maximum, Keyboard Application |
| `05 07` | Usage Page, Keyboard/Keypad |
| `19 00` | Usage Minimum, Reserved |
| `29 65` | Usage Maximum, Keyboard Application |
| `81 00` | Input, six key array |
| `C0` | End Collection |

Hex:

```text
05 01 09 06 A1 01 05 07 19 E0 29 E7 15 00 25 01
75 01 95 08 81 02 95 01 75 08 81 03 95 05 75 01
05 08 19 01 29 05 91 02 95 01 75 03 91 03 95 06
75 08 15 00 25 65 05 07 19 00 29 65 81 00 C0
```

## Composite Mouse + Keyboard

The first composite model uses two separate HID interfaces and no report IDs.

Interface order is deterministic:

1. mouse interface
2. keyboard interface

Endpoint order is deterministic:

1. mouse interrupt IN endpoint `0x81`
2. keyboard interrupt IN endpoint `0x82`

### Descriptor Order

1. Device descriptor
2. Configuration descriptor
3. Interface descriptor, mouse HID, interface `0`
4. HID descriptor, mouse report descriptor reference
5. Endpoint descriptor, mouse interrupt IN `0x81`
6. Interface descriptor, keyboard HID, interface `1`
7. HID descriptor, keyboard report descriptor reference
8. Endpoint descriptor, keyboard interrupt IN `0x82`
9. String descriptor 0, language ID
10. String descriptor 1, manufacturer
11. String descriptor 2, product

### Configuration Lengths

| Field | Value |
|---|---:|
| `wTotalLength` | `59` |
| `bNumInterfaces` | `2` |

Calculation: `9 configuration + 2 * (9 interface + 9 HID + 7 endpoint) = 59`.

### Composite Interface Values

| Function | Interface | Subclass | Protocol | Endpoint |
|---|---:|---:|---:|---:|
| Mouse | `0` | `0x00` | `0x00` | `0x81` |
| Keyboard | `1` | `0x01` | `0x01` | `0x82` |

## String Descriptor Layout

String descriptor behavior is defined by ADR 0010.

| Index | Contents |
|---:|---|
| `0` | Language ID descriptor: `04 03 09 04` |
| `1` | Manufacturer, printable ASCII encoded as UTF-16LE |
| `2` | Product, printable ASCII encoded as UTF-16LE |

## Golden Test Requirements

Golden tests for this RFC must verify:

- device descriptor bytes
- configuration descriptor bytes
- interface descriptor bytes
- HID descriptor bytes
- endpoint descriptor bytes
- string descriptor bytes
- HID report descriptor bytes
- `bLength` for every descriptor
- `wTotalLength`
- `bNumInterfaces`
- HID report descriptor length fields
- endpoint address, packet size, and interval
- absence of HID report IDs

## Assumptions to Verify During Implementation

- `bMaxPacketSize0 = 64` matches the Pico SDK / TinyUSB configuration used by year-one examples.
- `wMaxPacketSize = 8` for HID interrupt IN endpoints is compatible with TinyUSB's HID endpoint buffer configuration.
- The report-protocol mouse with wheel should use non-boot subclass/protocol values as specified here.
