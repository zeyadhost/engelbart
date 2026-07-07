#include "engelbart/engelbart.hpp"

using keyboard_identity = engelbart::identity<0x1209, 0x0100,
                                              engelbart::fixed_string{"Engelbart Example"},
                                              engelbart::fixed_string{"Example Keyboard"}>;
using invalid_device = engelbart::boot_keyboard<keyboard_identity, true, false>;

constexpr auto descriptor = engelbart::configuration_descriptor<invalid_device>();

int main() {
    return static_cast<int>(descriptor.size());
}
