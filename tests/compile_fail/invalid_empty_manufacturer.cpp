#include "engelbart/engelbart.hpp"

using invalid_identity = engelbart::identity<0x1209, 0x0100,
                                             engelbart::fixed_string{""},
                                             engelbart::fixed_string{"Product"}>;
using invalid_device = engelbart::hid_mouse<invalid_identity>;

constexpr auto descriptor = engelbart::manufacturer_string_descriptor<invalid_device>();

int main() {
    return static_cast<int>(descriptor.size());
}
