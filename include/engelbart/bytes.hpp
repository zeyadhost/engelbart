#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace engelbart {

using byte = std::uint8_t;

template <std::size_t Capacity>
struct byte_writer {
    std::array<byte, Capacity> data{};
    std::size_t offset = 0;

    constexpr void u8(byte value) {
        data[offset++] = value;
    }

    constexpr void u16le(std::uint16_t value) {
        u8(static_cast<byte>(value & 0xffu));
        u8(static_cast<byte>((value >> 8u) & 0xffu));
    }

    template <std::size_t N>
    constexpr void append(const std::array<byte, N>& bytes) {
        for (byte value : bytes) {
            u8(value);
        }
    }

    constexpr std::array<byte, Capacity> finish() const {
        return data;
    }
};

template <std::size_t N>
constexpr bool all_bytes_written(const byte_writer<N>& writer) {
    return writer.offset == N;
}

template <std::size_t N>
constexpr std::array<byte, N> concat() {
    return {};
}

template <std::size_t A, std::size_t B, std::size_t... Rest>
constexpr auto concat(const std::array<byte, A>& first,
                      const std::array<byte, B>& second,
                      const std::array<byte, Rest>&... rest) {
    std::array<byte, A + B + (Rest + ... + 0)> out{};
    std::size_t pos = 0;
    for (byte value : first) {
        out[pos++] = value;
    }
    for (byte value : second) {
        out[pos++] = value;
    }
    (([&] {
        for (byte value : rest) {
            out[pos++] = value;
        }
    }()), ...);
    return out;
}

template <std::size_t N>
constexpr byte descriptor_length(const std::array<byte, N>&) {
    static_assert(N <= 255);
    return static_cast<byte>(N);
}

}  // namespace engelbart
