#include <iostream>

#include "engelbart/engelbart.hpp"

using example_identity = engelbart::identity<0x1209, 0x0001,
                                             engelbart::fixed_string{"Engelbart Example"},
                                             engelbart::fixed_string{"Example HID Mouse"}>;
using example_device = engelbart::hid_mouse<example_identity>;

int main() {
    static_assert(engelbart::validate_device<example_device>().ok());
    std::cout << engelbart::inspect_device<example_device>();
}
