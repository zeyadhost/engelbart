#include "engelbart/engelbart.hpp"

using mouse_identity = engelbart::identity<0x1209, 0x0100,
                                           engelbart::fixed_string{"Engelbart Example"},
                                           engelbart::fixed_string{"Example Mouse"}>;
using invalid_device = engelbart::hid_mouse<mouse_identity, 5, true>;

constexpr auto descriptor = engelbart::configuration_descriptor<invalid_device>();

int main() {
    return static_cast<int>(descriptor.size());
}
