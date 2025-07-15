#pragma once

#include <cstddef>

// Utility functions
namespace Utils {
    namespace Memory {
        template <size_t N>
        constexpr size_t align_up(size_t value) {
            static_assert((N & (N - 1)) == 0, "N must be a power of two");
            return (value + N - 1) & ~(N - 1);
        }
    }
}