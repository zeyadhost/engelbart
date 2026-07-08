#include <algorithm>
#include <array>
#include <iostream>
#include <span>
#include <string>
#include <string_view>

#include "engelbart/engelbart.hpp"
#include "golden_descriptors.hpp"

namespace {

using mouse_id = engelbart::identity<0x1209, 0x0001,
                                     engelbart::fixed_string{"Engelbart Example"},
                                     engelbart::fixed_string{"Example HID Mouse"}>;
using keyboard_id = engelbart::identity<0x1209, 0x0002,
                                        engelbart::fixed_string{"Engelbart Example"},
                                        engelbart::fixed_string{"Example HID Keyboard"}>;
using composite_id = engelbart::identity<0x1209, 0x0003,
                                         engelbart::fixed_string{"Engelbart Example"},
                                         engelbart::fixed_string{"Example Composite HID"}>;

using mouse_device = engelbart::hid_mouse<mouse_id>;
using keyboard_device = engelbart::boot_keyboard<keyboard_id>;
using composite_device = engelbart::composite_mouse_keyboard<composite_id>;

int failures = 0;

void expect(bool condition, std::string_view message) {
    if (!condition) {
        ++failures;
        std::cerr << "FAIL: " << message << '\n';
    }
}

template <std::size_t A, std::size_t B>
void expect_bytes(const std::array<engelbart::byte, A>& actual,
                  const std::array<engelbart::byte, B>& expected,
                  std::string_view message) {
    if constexpr (A != B) {
        ++failures;
        std::cerr << "FAIL: " << message << " length " << A << " != " << B << '\n';
    } else if (!std::equal(actual.begin(), actual.end(), expected.begin())) {
        ++failures;
        std::cerr << "FAIL: " << message << '\n';
        std::cerr << "  actual:   " << engelbart::hex_dump(actual) << '\n';
        std::cerr << "  expected: " << engelbart::hex_dump(expected) << '\n';
    }
}

void test_byte_writer() {
    engelbart::byte_writer<3> out{};
    out.u8(0xaa);
    out.u16le(0x1234);
    expect_bytes(out.finish(), std::array<engelbart::byte, 3>{0xaa, 0x34, 0x12},
                 "byte writer emits little-endian values");
    expect(engelbart::all_bytes_written(out), "byte writer tracks length");
}

void test_device_descriptors() {
    expect_bytes(engelbart::device_descriptor<mouse_device>(),
                 engelbart_tests::golden::mouse_device_descriptor,
                 "mouse device descriptor");

    expect_bytes(engelbart::device_descriptor<keyboard_device>(),
                 engelbart_tests::golden::keyboard_device_descriptor,
                 "keyboard device descriptor");
}

void test_configuration_descriptors() {
    expect_bytes(engelbart::configuration_descriptor<mouse_device>(),
                 engelbart_tests::golden::mouse_configuration_descriptor,
                 "mouse configuration descriptor");

    expect_bytes(engelbart::configuration_descriptor<keyboard_device>(),
                 engelbart_tests::golden::keyboard_configuration_descriptor,
                 "keyboard configuration descriptor");

    const auto composite = engelbart::configuration_descriptor<composite_device>();
    expect(composite.size() == 59, "composite configuration descriptor length");
    expect(composite[2] == 0x3b && composite[3] == 0x00, "composite wTotalLength");
    expect(composite[4] == 2, "composite bNumInterfaces");
    expect(composite[11] == 0, "mouse interface number");
    expect(composite[36] == 1, "keyboard interface number");
    expect(composite[29] == 0x81, "mouse endpoint address");
    expect(composite[54] == 0x82, "keyboard endpoint address");
    expect(composite[33] == 0x0a && composite[58] == 0x0a, "endpoint intervals");
}

void test_descriptor_invariants() {
    const auto mouse_device_descriptor = engelbart::device_descriptor<mouse_device>();
    const auto keyboard_device_descriptor = engelbart::device_descriptor<keyboard_device>();
    const auto composite_device_descriptor = engelbart::device_descriptor<composite_device>();
    expect(mouse_device_descriptor[17] == 1, "mouse bNumConfigurations");
    expect(keyboard_device_descriptor[17] == 1, "keyboard bNumConfigurations");
    expect(composite_device_descriptor[17] == 1, "composite bNumConfigurations");
    expect(mouse_device_descriptor[14] == 1 && mouse_device_descriptor[15] == 2 &&
               mouse_device_descriptor[16] == 0,
           "mouse string indexes");
    expect(keyboard_device_descriptor[14] == 1 && keyboard_device_descriptor[15] == 2 &&
               keyboard_device_descriptor[16] == 0,
           "keyboard string indexes");
    expect(composite_device_descriptor[14] == 1 && composite_device_descriptor[15] == 2 &&
               composite_device_descriptor[16] == 0,
           "composite string indexes");

    const auto mouse = engelbart::configuration_descriptor<mouse_device>();
    const auto keyboard = engelbart::configuration_descriptor<keyboard_device>();
    const auto composite = engelbart::configuration_descriptor<composite_device>();
    expect(mouse[4] == 1, "mouse bNumInterfaces");
    expect(keyboard[4] == 1, "keyboard bNumInterfaces");
    expect(composite[4] == 2, "composite bNumInterfaces");

    expect(mouse[25] == engelbart::mouse_report_descriptor.size() && mouse[26] == 0,
           "mouse HID report length");
    expect(keyboard[25] == engelbart::keyboard_report_descriptor.size() && keyboard[26] == 0,
           "keyboard HID report length");
    expect(composite[25] == engelbart::mouse_report_descriptor.size() && composite[26] == 0,
           "composite mouse HID report length");
    expect(composite[50] == engelbart::keyboard_report_descriptor.size() && composite[51] == 0,
           "composite keyboard HID report length");

    expect(mouse[31] == 8 && mouse[32] == 0 && mouse[33] == 10,
           "mouse endpoint packet size and interval");
    expect(keyboard[31] == 8 && keyboard[32] == 0 && keyboard[33] == 10,
           "keyboard endpoint packet size and interval");
    expect(composite[31] == 8 && composite[32] == 0 && composite[33] == 10,
           "composite mouse endpoint packet size and interval");
    expect(composite[56] == 8 && composite[57] == 0 && composite[58] == 10,
           "composite keyboard endpoint packet size and interval");
}

void test_hid_report_descriptors() {
    expect_bytes(engelbart::mouse_report_descriptor,
                 engelbart_tests::golden::mouse_report_descriptor,
                 "mouse report descriptor");

    expect_bytes(engelbart::keyboard_report_descriptor,
                 engelbart_tests::golden::keyboard_report_descriptor,
                 "keyboard report descriptor");
}

void test_string_descriptors() {
    expect_bytes(engelbart::language_id_string_descriptor(),
                 std::array<engelbart::byte, 4>{0x04, 0x03, 0x09, 0x04},
                 "language ID descriptor");

    using short_id = engelbart::identity<0x1209, 0x0100,
                                         engelbart::fixed_string{"AB"},
                                         engelbart::fixed_string{"CD"}>;
    using short_mouse = engelbart::hid_mouse<short_id>;
    expect_bytes(engelbart::manufacturer_string_descriptor<short_mouse>(),
                 std::array<engelbart::byte, 6>{0x06, 0x03, 0x41, 0x00, 0x42, 0x00},
                 "manufacturer UTF-16LE bytes");
    expect_bytes(engelbart::product_string_descriptor<short_mouse>(),
                 std::array<engelbart::byte, 6>{0x06, 0x03, 0x43, 0x00, 0x44, 0x00},
                 "product UTF-16LE bytes");

    using max_string_id = engelbart::identity<0x1209, 0x0101,
                                              engelbart::fixed_string{"1234567890123456789012345678901"},
                                              engelbart::fixed_string{"Product"}>;
    using max_string_mouse = engelbart::hid_mouse<max_string_id>;
    expect(engelbart::validate_device<max_string_mouse>().ok(), "31-character string accepted");
    expect(engelbart::manufacturer_string_descriptor<max_string_mouse>().size() == 64,
           "31-character string descriptor length");

    expect(engelbart::descriptor_set<mouse_device>::device[14] == 1, "manufacturer index");
    expect(engelbart::descriptor_set<mouse_device>::device[15] == 2, "product index");
    expect(engelbart::descriptor_set<mouse_device>::device[16] == 0, "serial omitted");
}

void test_allocators() {
    const auto composite_endpoints = engelbart::endpoint_allocation_for<composite_device>();
    expect(composite_endpoints.count == 2, "composite endpoint count");
    expect(composite_endpoints.in_addresses[0] == 0x81, "first endpoint");
    expect(composite_endpoints.in_addresses[1] == 0x82, "second endpoint");

    const auto interfaces = engelbart::interface_allocation_for<composite_device>();
    expect(interfaces.count == 2, "composite interface count");
    expect(interfaces.numbers[0] == 0, "first interface");
    expect(interfaces.numbers[1] == 1, "second interface");

    const auto exhausted = engelbart::allocate_hid_in_endpoints(5);
    expect(!exhausted.validation.ok(), "endpoint exhaustion fails");
    expect(exhausted.validation.contains("EGB-ENDPOINT-001"), "endpoint diagnostic ID");
}

void test_validation() {
    using empty_string_id = engelbart::identity<0x1209, 0x0100,
                                                engelbart::fixed_string{""},
                                                engelbart::fixed_string{"Product"}>;
    using empty_string_mouse = engelbart::hid_mouse<empty_string_id>;
    expect(engelbart::validate_device<empty_string_mouse>().contains("EGB-STR-001"),
           "empty manufacturer diagnostic");

    using empty_product_id = engelbart::identity<0x1209, 0x0100,
                                                engelbart::fixed_string{"Maker"},
                                                engelbart::fixed_string{""}>;
    using empty_product_mouse = engelbart::hid_mouse<empty_product_id>;
    expect(engelbart::validate_device<empty_product_mouse>().contains("EGB-STR-002"),
           "empty product diagnostic");

    using too_long_string_id = engelbart::identity<0x1209, 0x0100,
                                                  engelbart::fixed_string{"12345678901234567890123456789012"},
                                                  engelbart::fixed_string{"Product"}>;
    using too_long_string_mouse = engelbart::hid_mouse<too_long_string_id>;
    expect(engelbart::validate_device<too_long_string_mouse>().contains("EGB-STR-001"),
           "32-character manufacturer diagnostic");

    using control_char_id = engelbart::identity<0x1209, 0x0100,
                                                engelbart::fixed_string{"Bad\x1f"},
                                                engelbart::fixed_string{"Product"}>;
    using control_char_mouse = engelbart::hid_mouse<control_char_id>;
    expect(engelbart::validate_device<control_char_mouse>().contains("EGB-STR-001"),
           "control character diagnostic");

    using bad_char_id = engelbart::identity<0x1209, 0x0100,
                                            engelbart::fixed_string{"Bad\x7f"},
                                            engelbart::fixed_string{"Product"}>;
    using bad_char_mouse = engelbart::hid_mouse<bad_char_id>;
    expect(engelbart::validate_device<bad_char_mouse>().contains("EGB-STR-001"),
           "invalid character diagnostic");

    using non_ascii_id = engelbart::identity<0x1209, 0x0100,
                                             engelbart::fixed_string{"Cafe\xC3\xA9"},
                                             engelbart::fixed_string{"Product"}>;
    using non_ascii_mouse = engelbart::hid_mouse<non_ascii_id>;
    expect(engelbart::validate_device<non_ascii_mouse>().contains("EGB-STR-001"),
           "non-ASCII diagnostic");

    using bad_vid_id = engelbart::identity<0x0000, 0x0100,
                                           engelbart::fixed_string{"Maker"},
                                           engelbart::fixed_string{"Product"}>;
    using bad_vid_mouse = engelbart::hid_mouse<bad_vid_id>;
    expect(engelbart::validate_device<bad_vid_mouse>().contains("EGB-ID-001"),
           "invalid VID diagnostic");

    using bad_pid_id = engelbart::identity<0x1209, 0x0000,
                                           engelbart::fixed_string{"Maker"},
                                           engelbart::fixed_string{"Product"}>;
    using bad_pid_mouse = engelbart::hid_mouse<bad_pid_id>;
    expect(engelbart::validate_device<bad_pid_mouse>().contains("EGB-ID-001"),
           "invalid PID diagnostic");

    using unsupported_keyboard = engelbart::boot_keyboard<keyboard_id, true, false>;
    expect(engelbart::validate_device<unsupported_keyboard>().contains("EGB-UNSUPPORTED-001"),
           "unsupported keyboard option diagnostic");

    using unsupported_mouse = engelbart::hid_mouse<mouse_id, 5, true>;
    expect(engelbart::validate_device<unsupported_mouse>().contains("EGB-HID-001"),
           "unsupported mouse model diagnostic");

    const auto tinyusb_mismatch = engelbart::validate_tinyusb_config<composite_device>(1, 64, 8);
    expect(tinyusb_mismatch.contains("EGB-TINYUSB-001"),
           "TinyUSB HID instance mismatch diagnostic");

    auto bad_config = engelbart::configuration_descriptor<mouse_device>();
    bad_config[2] = 0x21;
    const auto bad_config_result =
        engelbart::validate_configuration_descriptor_bytes(std::span<const engelbart::byte>{
            bad_config.data(), bad_config.size()});
    expect(bad_config_result.contains("EGB-DESC-002"),
           "descriptor length mismatch diagnostic");
}

void test_tinyusb_glue() {
    using glue = engelbart::tinyusb_glue<composite_device>;
    expect(glue::hid_instance_count == 2, "composite TinyUSB HID instance count");
    expect(glue::device_callback().size() == 18, "device callback size");
    expect(glue::configuration_callback().size() == 59, "configuration callback size");
    expect(glue::string_callback(0).size() == 4, "string index 0");
    expect(glue::string_callback(1).size() == engelbart::descriptor_set<composite_device>::manufacturer.size(),
           "string index 1");
    expect(glue::string_callback(2).size() == engelbart::descriptor_set<composite_device>::product.size(),
           "string index 2");
    expect(glue::string_callback(3).empty(), "unsupported string index");
    expect(glue::hid_report_callback(0).size() == engelbart::mouse_report_descriptor.size(),
           "mouse report callback");
    expect(glue::hid_report_callback(1).size() == engelbart::keyboard_report_descriptor.size(),
           "keyboard report callback");
    expect(glue::hid_report_callback(2).empty(), "unsupported report instance");
}

void test_inspection() {
    const auto text = engelbart::inspect_device<composite_device>();
    expect(text.find("device_descriptor.hex") != std::string::npos, "inspection device hex");
    expect(text.find("configuration_descriptor.fields") != std::string::npos,
           "inspection decoded config fields");
    expect(text.find("endpoint_allocation") != std::string::npos, "inspection endpoints");
    expect(text.find("interface_numbering") != std::string::npos, "inspection interfaces");
    expect(text.find("string_descriptors") != std::string::npos, "inspection strings");
    expect(text.find("tinyusb_callbacks") != std::string::npos, "inspection TinyUSB map");
}

}  // namespace

int main() {
    test_byte_writer();
    test_device_descriptors();
    test_configuration_descriptors();
    test_descriptor_invariants();
    test_hid_report_descriptors();
    test_string_descriptors();
    test_allocators();
    test_validation();
    test_tinyusb_glue();
    test_inspection();

    if (failures != 0) {
        std::cerr << failures << " test failure(s)\n";
        return 1;
    }

    std::cout << "engelbart_tests passed\n";
    return 0;
}
