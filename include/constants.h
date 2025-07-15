#pragma once

#include <cstdlib>

// Alignment optimisation
namespace Constants {
    constexpr size_t CACHE_LINE_SIZE = 64;
    constexpr size_t PAGE_SIZE = 4096;
    constexpr size_t STANDARD_ALIGNMENT = 8;
}