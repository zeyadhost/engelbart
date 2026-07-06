#include <iostream>

#include "engelbart/engelbart.hpp"

using example_identity = engelbart::identity<0x1209, 0x0003,
                                             engelbart::fixed_string{"Engelbart Example"},
                                             engelbart::fixed_string{"Example Composite HID"}>;
using example_device = engelbart::composite_mouse_keyboard<example_identity>;

int main() {
    static_assert(engelbart::validate_device<example_device>().ok());
    std::cout << engelbart::inspect_device<example_device>();
}
