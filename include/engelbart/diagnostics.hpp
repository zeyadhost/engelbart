#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

#include "engelbart/model.hpp"

namespace engelbart {

struct diagnostic {
    std::string_view id;
    std::string_view message;
};

template <std::size_t MaxDiagnostics = 8>
struct validation_result {
    std::array<diagnostic, MaxDiagnostics> diagnostics{};
    std::size_t count = 0;

    constexpr bool ok() const {
        return count == 0;
    }

    constexpr void add(std::string_view id, std::string_view message) {
        if (count < MaxDiagnostics) {
            diagnostics[count++] = diagnostic{id, message};
        }
    }

    constexpr bool contains(std::string_view id) const {
        for (std::size_t i = 0; i < count; ++i) {
            if (diagnostics[i].id == id) {
                return true;
            }
        }
        return false;
    }
};

template <fixed_string Text>
constexpr bool valid_usb_string() {
    if constexpr (Text.size() == 0 || Text.size() > 31) {
        return false;
    }

    for (std::size_t i = 0; i < Text.size(); ++i) {
        const auto ch = static_cast<unsigned char>(Text[i]);
        if (ch < 0x20u || ch > 0x7eu) {
            return false;
        }
    }
    return true;
}

template <typename Identity>
constexpr void validate_identity(validation_result<>& result) {
    if constexpr (Identity::vendor_id == 0 || Identity::product_id == 0) {
        result.add("EGB-ID-001", "VID and PID must be non-zero in the minimal model");
    }

    if constexpr (!valid_usb_string<Identity::manufacturer>()) {
        result.add("EGB-STR-001", "manufacturer must be 1..31 printable ASCII characters");
    }

    if constexpr (!valid_usb_string<Identity::product>()) {
        result.add("EGB-STR-002", "product must be 1..31 printable ASCII characters");
    }
}

constexpr validation_result<> validate_hid_in_endpoint_count(std::size_t requested) {
    validation_result<> result{};
    if (requested > 4) {
        result.add("EGB-ENDPOINT-001",
                   "endpoint budget exceeded for target pico_wh");
    }
    return result;
}

template <typename Device>
constexpr validation_result<> validate_device() {
    validation_result<> result{};
    using Identity = typename device_traits<Device>::identity_type;
    validate_identity<Identity>(result);

    const auto endpoint_result =
        validate_hid_in_endpoint_count(device_traits<Device>::hid_in_endpoint_count);
    for (std::size_t i = 0; i < endpoint_result.count; ++i) {
        result.add(endpoint_result.diagnostics[i].id, endpoint_result.diagnostics[i].message);
    }

    if constexpr (requires { Device::buttons; Device::wheel; }) {
        if constexpr (Device::buttons != 3 || !Device::wheel) {
            result.add("EGB-HID-001",
                       "year-one mouse model supports exactly three buttons and wheel");
        }
    }

    if constexpr (requires { Device::nkro; Device::media_keys; }) {
        if constexpr (Device::nkro || Device::media_keys) {
            result.add("EGB-UNSUPPORTED-001",
                       "NKRO and media keys are deferred for the year-one keyboard model");
        }
    }

    return result;
}

template <typename Device>
constexpr void require_valid_device() {
    static_assert(validate_device<Device>().ok(),
                  "EGB-VALIDATION: invalid Engelbart device description; call validate_device for diagnostic IDs");
}

}  // namespace engelbart
