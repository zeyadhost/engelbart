#include <array>
#include <cstddef>
#include <cstdint>

#include "pico/stdlib.h"
#include "pico/usb_reset_interface.h"
#include "tusb.h"

#include "engelbart/engelbart.hpp"

using example_identity = engelbart::identity<0x1209, 0x0003,
                                             engelbart::fixed_string{"Engelbart Example"},
                                             engelbart::fixed_string{"Example Composite HID"}>;
using example_device = engelbart::composite_mouse_keyboard<example_identity>;
using glue = engelbart::tinyusb_glue<example_device>;

static_assert(engelbart::validate_device<example_device>().ok());
static_assert(glue::hid_instance_count == 2);

namespace {

// Smoke-firmware-only reset interface for picotool development workflows.
inline constexpr std::array<engelbart::byte, 9> reset_interface_descriptor{
    9, 0x04, 2, 0, 0, 0xff, RESET_INTERFACE_SUBCLASS, RESET_INTERFACE_PROTOCOL, 0};

constexpr auto smoke_configuration_descriptor() {
    constexpr auto hid_configuration = engelbart::descriptor_set<example_device>::configuration;
    std::array<engelbart::byte,
               hid_configuration.size() + reset_interface_descriptor.size()> out{};

    std::size_t offset = 0;
    for (const auto value : hid_configuration) {
        out[offset++] = value;
    }
    for (const auto value : reset_interface_descriptor) {
        out[offset++] = value;
    }

    out[2] = static_cast<engelbart::byte>(out.size() & 0xffu);
    out[3] = static_cast<engelbart::byte>((out.size() >> 8u) & 0xffu);
    out[4] = 3;
    return out;
}

inline constexpr auto smoke_configuration = smoke_configuration_descriptor();

}  // namespace

extern "C" const std::uint8_t* tud_descriptor_device_cb() {
    return glue::device_callback().data();
}

extern "C" const std::uint8_t* tud_descriptor_configuration_cb(std::uint8_t) {
    return smoke_configuration.data();
}

extern "C" const std::uint8_t* tud_hid_descriptor_report_cb(std::uint8_t instance) {
    return glue::hid_report_callback(instance).data();
}

extern "C" const std::uint16_t* tud_descriptor_string_cb(std::uint8_t index, std::uint16_t) {
    static std::array<std::uint16_t, 32> buffer{};
    const auto descriptor = glue::string_callback(index);
    if (descriptor.empty()) {
        return nullptr;
    }

    const auto byte_count = descriptor[0];
    buffer[0] = static_cast<std::uint16_t>((0x03u << 8u) | byte_count);
    for (std::size_t i = 2, out = 1; i + 1 < byte_count; i += 2, ++out) {
        buffer[out] = static_cast<std::uint16_t>(descriptor[i] | (descriptor[i + 1] << 8u));
    }
    return buffer.data();
}

extern "C" std::uint16_t tud_hid_get_report_cb(std::uint8_t,
                                                std::uint8_t,
                                                hid_report_type_t,
                                                std::uint8_t*,
                                                std::uint16_t) {
    return 0;
}

extern "C" void tud_hid_set_report_cb(std::uint8_t,
                                       std::uint8_t,
                                       hid_report_type_t,
                                       const std::uint8_t*,
                                       std::uint16_t) {}

int main() {
    stdio_init_all();
    tusb_init();
    while (true) {
        tud_task();
    }
}
