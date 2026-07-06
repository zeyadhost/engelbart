#include <array>
#include <cstddef>
#include <cstdint>

#include "pico/stdlib.h"
#include "pico/unique_id.h"
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

inline constexpr std::size_t device_descriptor_serial_index_offset = 16;
inline constexpr engelbart::byte smoke_serial_string_index = 3;

// Smoke-firmware-only reset interface for picotool development workflows.
inline constexpr std::array<engelbart::byte, 9> reset_interface_descriptor{
    9, 0x04, 2, 0, 0, 0xff, RESET_INTERFACE_SUBCLASS, RESET_INTERFACE_PROTOCOL, 0};

constexpr auto smoke_device_descriptor() {
    auto descriptor = engelbart::descriptor_set<example_device>::device;
    descriptor[device_descriptor_serial_index_offset] = smoke_serial_string_index;
    return descriptor;
}

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

inline constexpr auto smoke_device = smoke_device_descriptor();
inline constexpr auto smoke_configuration = smoke_configuration_descriptor();

}  // namespace

extern "C" const std::uint8_t* tud_descriptor_device_cb() {
    return smoke_device.data();
}

extern "C" const std::uint8_t* tud_descriptor_configuration_cb(std::uint8_t) {
    return smoke_configuration.data();
}

extern "C" const std::uint8_t* tud_hid_descriptor_report_cb(std::uint8_t instance) {
    return glue::hid_report_callback(instance).data();
}

extern "C" const std::uint16_t* tud_descriptor_string_cb(std::uint8_t index, std::uint16_t) {
    static std::array<std::uint16_t, 32> buffer{};
    static char serial[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2 + 1]{};

    if (index == smoke_serial_string_index) {
        if (serial[0] == '\0') {
            pico_get_unique_board_id_string(serial, sizeof(serial));
        }

        std::size_t len = 0;
        while (len + 1 < buffer.size() && serial[len] != '\0') {
            buffer[len + 1] = static_cast<std::uint16_t>(serial[len]);
            ++len;
        }
        buffer[0] = static_cast<std::uint16_t>((0x03u << 8u) | (2u * len + 2u));
        return buffer.data();
    }

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
