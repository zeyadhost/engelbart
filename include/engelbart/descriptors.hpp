#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

#include "engelbart/allocators.hpp"
#include "engelbart/bytes.hpp"
#include "engelbart/diagnostics.hpp"
#include "engelbart/hid.hpp"
#include "engelbart/model.hpp"

namespace engelbart {

template <typename Identity>
constexpr std::array<byte, 18> device_descriptor_for_identity() {
    byte_writer<18> out{};
    out.u8(18);
    out.u8(0x01);
    out.u16le(0x0200);
    out.u8(0x00);
    out.u8(0x00);
    out.u8(0x00);
    out.u8(64);
    out.u16le(Identity::vendor_id);
    out.u16le(Identity::product_id);
    out.u16le(0x0100);
    out.u8(1);
    out.u8(2);
    out.u8(0);
    out.u8(1);
    return out.finish();
}

template <typename Device>
constexpr auto device_descriptor() {
    require_valid_device<Device>();
    using Identity = typename device_traits<Device>::identity_type;
    return device_descriptor_for_identity<Identity>();
}

constexpr std::array<byte, 9> configuration_header(std::uint16_t total_length,
                                                   std::uint8_t interface_count) {
    byte_writer<9> out{};
    out.u8(9);
    out.u8(0x02);
    out.u16le(total_length);
    out.u8(interface_count);
    out.u8(1);
    out.u8(0);
    out.u8(0x80);
    out.u8(50);
    return out.finish();
}

constexpr std::array<byte, 9> interface_descriptor(std::uint8_t number,
                                                   std::uint8_t subclass,
                                                   std::uint8_t protocol) {
    byte_writer<9> out{};
    out.u8(9);
    out.u8(0x04);
    out.u8(number);
    out.u8(0);
    out.u8(1);
    out.u8(0x03);
    out.u8(subclass);
    out.u8(protocol);
    out.u8(0);
    return out.finish();
}

constexpr std::array<byte, 9> hid_descriptor(std::uint16_t report_length) {
    byte_writer<9> out{};
    out.u8(9);
    out.u8(0x21);
    out.u16le(0x0111);
    out.u8(0);
    out.u8(1);
    out.u8(0x22);
    out.u16le(report_length);
    return out.finish();
}

constexpr std::array<byte, 7> endpoint_descriptor(std::uint8_t address) {
    byte_writer<7> out{};
    out.u8(7);
    out.u8(0x05);
    out.u8(address);
    out.u8(0x03);
    out.u16le(8);
    out.u8(10);
    return out.finish();
}

constexpr auto mouse_interface_group(std::uint8_t interface_number,
                                     std::uint8_t endpoint_address) {
    return concat(interface_descriptor(interface_number, 0x00, 0x00),
                  hid_descriptor(static_cast<std::uint16_t>(mouse_report_descriptor.size())),
                  endpoint_descriptor(endpoint_address));
}

constexpr auto keyboard_interface_group(std::uint8_t interface_number,
                                        std::uint8_t endpoint_address) {
    return concat(interface_descriptor(interface_number, 0x01, 0x01),
                  hid_descriptor(static_cast<std::uint16_t>(keyboard_report_descriptor.size())),
                  endpoint_descriptor(endpoint_address));
}

template <typename Device>
constexpr auto configuration_descriptor() {
    require_valid_device<Device>();
    if constexpr (requires { Device::buttons; Device::wheel; }) {
        return concat(configuration_header(34, 1), mouse_interface_group(0, 0x81));
    } else if constexpr (requires { Device::nkro; Device::media_keys; }) {
        return concat(configuration_header(34, 1), keyboard_interface_group(0, 0x81));
    } else {
        return concat(configuration_header(59, 2),
                      mouse_interface_group(0, 0x81),
                      keyboard_interface_group(1, 0x82));
    }
}

template <typename Device>
constexpr std::size_t hid_report_descriptor_count() {
    require_valid_device<Device>();
    if constexpr (requires { Device::buttons; Device::wheel; }) {
        return 1;
    } else if constexpr (requires { Device::nkro; Device::media_keys; }) {
        return 1;
    } else {
        return 2;
    }
}

constexpr std::array<byte, 4> language_id_string_descriptor() {
    return {0x04, 0x03, 0x09, 0x04};
}

template <fixed_string Text>
constexpr auto ascii_string_descriptor() {
    static_assert(valid_usb_string<Text>(), "EGB-STR-001: invalid USB string");
    byte_writer<2 + Text.size() * 2> out{};
    out.u8(static_cast<byte>(2 + Text.size() * 2));
    out.u8(0x03);
    for (std::size_t i = 0; i < Text.size(); ++i) {
        out.u8(static_cast<byte>(Text[i]));
        out.u8(0x00);
    }
    return out.finish();
}

template <typename Device>
constexpr auto manufacturer_string_descriptor() {
    using Identity = typename device_traits<Device>::identity_type;
    return ascii_string_descriptor<Identity::manufacturer>();
}

template <typename Device>
constexpr auto product_string_descriptor() {
    using Identity = typename device_traits<Device>::identity_type;
    return ascii_string_descriptor<Identity::product>();
}

template <typename Device>
struct descriptor_set {
    static inline constexpr auto device = device_descriptor<Device>();
    static inline constexpr auto configuration = configuration_descriptor<Device>();
    static inline constexpr auto language = language_id_string_descriptor();
    static inline constexpr auto manufacturer = manufacturer_string_descriptor<Device>();
    static inline constexpr auto product = product_string_descriptor<Device>();
};

}  // namespace engelbart
