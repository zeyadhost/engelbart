#pragma once

#include <array>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>

#include "engelbart/allocators.hpp"
#include "engelbart/descriptors.hpp"

namespace engelbart {

template <std::size_t N>
std::string hex_dump(const std::array<byte, N>& bytes) {
    std::ostringstream out;
    out << std::hex << std::uppercase << std::setfill('0');
    for (std::size_t i = 0; i < N; ++i) {
        if (i != 0) {
            out << ' ';
        }
        out << std::setw(2) << static_cast<unsigned>(bytes[i]);
    }
    return out.str();
}

inline std::uint16_t read_u16le(const byte* bytes) {
    return static_cast<std::uint16_t>(bytes[0] | (bytes[1] << 8u));
}

template <typename Device>
std::string inspect_device() {
    const auto endpoints = endpoint_allocation_for<Device>();
    const auto interfaces = interface_allocation_for<Device>();

    std::ostringstream out;
    out << "Engelbart inspection\n";
    out << "device: " << device_traits<Device>::name << "\n\n";

    out << "device_descriptor.hex: " << hex_dump(descriptor_set<Device>::device) << "\n";
    out << "device_descriptor.fields:\n";
    out << "  bLength: " << static_cast<unsigned>(descriptor_set<Device>::device[0]) << "\n";
    out << "  bDescriptorType: " << static_cast<unsigned>(descriptor_set<Device>::device[1]) << "\n";
    out << "  idVendor: 0x" << std::hex << std::uppercase
        << std::setw(4) << std::setfill('0')
        << read_u16le(&descriptor_set<Device>::device[8]) << std::dec << "\n";
    out << "  idProduct: 0x" << std::hex << std::uppercase
        << std::setw(4) << std::setfill('0')
        << read_u16le(&descriptor_set<Device>::device[10]) << std::dec << "\n\n";

    out << "configuration_descriptor.hex: "
        << hex_dump(descriptor_set<Device>::configuration) << "\n";
    out << "configuration_descriptor.fields:\n";
    out << "  wTotalLength: "
        << read_u16le(&descriptor_set<Device>::configuration[2]) << "\n";
    out << "  bNumInterfaces: "
        << static_cast<unsigned>(descriptor_set<Device>::configuration[4]) << "\n\n";

    out << "hid_report_descriptors:\n";
    if constexpr (requires { Device::buttons; Device::wheel; }) {
        out << "  instance 0: " << hex_dump(mouse_report_descriptor) << "\n";
    } else if constexpr (requires { Device::nkro; Device::media_keys; }) {
        out << "  instance 0: " << hex_dump(keyboard_report_descriptor) << "\n";
    } else {
        out << "  instance 0: " << hex_dump(mouse_report_descriptor) << "\n";
        out << "  instance 1: " << hex_dump(keyboard_report_descriptor) << "\n";
    }

    out << "endpoint_allocation:\n";
    for (std::size_t i = 0; i < endpoints.count; ++i) {
        out << "  hid_in[" << i << "]: 0x" << std::hex << std::uppercase
            << std::setw(2) << std::setfill('0')
            << static_cast<unsigned>(endpoints.in_addresses[i])
            << std::dec << " packet_size=8 interval=10\n";
    }

    out << "interface_numbering:\n";
    for (std::size_t i = 0; i < interfaces.count; ++i) {
        out << "  interface[" << i << "]: "
            << static_cast<unsigned>(interfaces.numbers[i]) << "\n";
    }

    out << "string_descriptors:\n";
    out << "  0: " << hex_dump(descriptor_set<Device>::language) << "\n";
    out << "  1: " << hex_dump(descriptor_set<Device>::manufacturer) << "\n";
    out << "  2: " << hex_dump(descriptor_set<Device>::product) << "\n";

    out << "tinyusb_callbacks:\n";
    out << "  tud_descriptor_device_cb -> device_descriptor\n";
    out << "  tud_descriptor_configuration_cb -> configuration_descriptor\n";
    out << "  tud_descriptor_string_cb -> string_descriptors[0..2]\n";
    out << "  tud_hid_descriptor_report_cb -> hid_report_descriptors\n";
    return out.str();
}

}  // namespace engelbart
