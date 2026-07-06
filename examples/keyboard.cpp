#include <iostream>

#include "engelbart/engelbart.hpp"

using example_identity = engelbart::identity<0x1209, 0x0002,
                                             engelbart::fixed_string{"Engelbart Example"},
                                             engelbart::fixed_string{"Example HID Keyboard"}>;
using example_device = engelbart::boot_keyboard<example_identity>;

int main() {
    static_assert(engelbart::validate_device<example_device>().ok());
    std::cout << engelbart::inspect_device<example_device>();
}
