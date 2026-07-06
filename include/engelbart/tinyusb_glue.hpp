#pragma once

#include <span>

#include "engelbart/descriptors.hpp"

namespace engelbart {

template <typename Device>
struct tinyusb_glue {
    static constexpr std::size_t hid_instance_count =
        device_traits<Device>::hid_in_endpoint_count;

    static constexpr std::span<const byte> device_callback() {
        return {descriptor_set<Device>::device.data(), descriptor_set<Device>::device.size()};
    }

    static constexpr std::span<const byte> configuration_callback() {
        return {descriptor_set<Device>::configuration.data(),
                descriptor_set<Device>::configuration.size()};
    }

    static constexpr std::span<const byte> string_callback(std::uint8_t index) {
        switch (index) {
            case 0:
                return {descriptor_set<Device>::language.data(),
                        descriptor_set<Device>::language.size()};
            case 1:
                return {descriptor_set<Device>::manufacturer.data(),
                        descriptor_set<Device>::manufacturer.size()};
            case 2:
                return {descriptor_set<Device>::product.data(),
                        descriptor_set<Device>::product.size()};
            default:
                return {};
        }
    }

    static constexpr std::span<const byte> hid_report_callback(std::uint8_t instance) {
        if constexpr (requires { Device::buttons; Device::wheel; }) {
            return instance == 0
                ? std::span<const byte>{mouse_report_descriptor.data(),
                                        mouse_report_descriptor.size()}
                : std::span<const byte>{};
        } else if constexpr (requires { Device::nkro; Device::media_keys; }) {
            return instance == 0
                ? std::span<const byte>{keyboard_report_descriptor.data(),
                                        keyboard_report_descriptor.size()}
                : std::span<const byte>{};
        } else {
            if (instance == 0) {
                return {mouse_report_descriptor.data(), mouse_report_descriptor.size()};
            }
            if (instance == 1) {
                return {keyboard_report_descriptor.data(), keyboard_report_descriptor.size()};
            }
            return {};
        }
    }
};

}  // namespace engelbart
