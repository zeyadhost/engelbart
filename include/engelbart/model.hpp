#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace engelbart {

template <std::size_t N>
struct fixed_string {
    char value[N]{};

    constexpr fixed_string(const char (&input)[N]) {
        for (std::size_t i = 0; i < N; ++i) {
            value[i] = input[i];
        }
    }

    constexpr std::size_t size() const {
        return N - 1;
    }

    constexpr char operator[](std::size_t index) const {
        return value[index];
    }

    constexpr std::string_view view() const {
        return {value, size()};
    }
};

template <std::size_t N>
fixed_string(const char (&)[N]) -> fixed_string<N>;

template <std::uint16_t VendorId,
          std::uint16_t ProductId,
          fixed_string Manufacturer,
          fixed_string Product>
struct identity {
    static constexpr std::uint16_t vendor_id = VendorId;
    static constexpr std::uint16_t product_id = ProductId;
    static constexpr auto manufacturer = Manufacturer;
    static constexpr auto product = Product;
};

template <typename Identity, std::uint8_t Buttons = 3, bool Wheel = true>
struct hid_mouse {
    using identity_type = Identity;
    static constexpr std::uint8_t buttons = Buttons;
    static constexpr bool wheel = Wheel;
};

template <typename Identity, bool Nkro = false, bool MediaKeys = false>
struct boot_keyboard {
    using identity_type = Identity;
    static constexpr bool nkro = Nkro;
    static constexpr bool media_keys = MediaKeys;
};

template <typename Identity>
struct composite_mouse_keyboard {
    using identity_type = Identity;
};

template <typename Device>
struct device_traits;

template <typename Identity, std::uint8_t Buttons, bool Wheel>
struct device_traits<hid_mouse<Identity, Buttons, Wheel>> {
    using identity_type = Identity;
    static constexpr std::string_view name = "hid_mouse";
    static constexpr std::size_t interface_count = 1;
    static constexpr std::size_t hid_in_endpoint_count = 1;
};

template <typename Identity, bool Nkro, bool MediaKeys>
struct device_traits<boot_keyboard<Identity, Nkro, MediaKeys>> {
    using identity_type = Identity;
    static constexpr std::string_view name = "boot_keyboard";
    static constexpr std::size_t interface_count = 1;
    static constexpr std::size_t hid_in_endpoint_count = 1;
};

template <typename Identity>
struct device_traits<composite_mouse_keyboard<Identity>> {
    using identity_type = Identity;
    static constexpr std::string_view name = "composite_mouse_keyboard";
    static constexpr std::size_t interface_count = 2;
    static constexpr std::size_t hid_in_endpoint_count = 2;
};

}  // namespace engelbart
