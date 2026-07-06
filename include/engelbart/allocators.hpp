#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "engelbart/diagnostics.hpp"

namespace engelbart {

struct endpoint_allocation {
    std::array<std::uint8_t, 4> in_addresses{};
    std::size_t count = 0;
    validation_result<> validation{};
};

constexpr endpoint_allocation allocate_hid_in_endpoints(std::size_t requested) {
    endpoint_allocation allocation{};
    allocation.count = requested <= 4 ? requested : 0;
    allocation.validation = validate_hid_in_endpoint_count(requested);
    if (!allocation.validation.ok()) {
        return allocation;
    }

    for (std::size_t i = 0; i < requested; ++i) {
        allocation.in_addresses[i] = static_cast<std::uint8_t>(0x81u + i);
    }
    return allocation;
}

struct interface_allocation {
    std::array<std::uint8_t, 2> numbers{};
    std::size_t count = 0;
};

constexpr interface_allocation allocate_interfaces(std::size_t requested) {
    interface_allocation allocation{};
    allocation.count = requested;
    for (std::size_t i = 0; i < requested && i < allocation.numbers.size(); ++i) {
        allocation.numbers[i] = static_cast<std::uint8_t>(i);
    }
    return allocation;
}

template <typename Device>
constexpr auto endpoint_allocation_for() {
    return allocate_hid_in_endpoints(device_traits<Device>::hid_in_endpoint_count);
}

template <typename Device>
constexpr auto interface_allocation_for() {
    return allocate_interfaces(device_traits<Device>::interface_count);
}

}  // namespace engelbart
