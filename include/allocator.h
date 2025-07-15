#pragma once

#include <cstddef>
#include <concepts>

// Base allocator concept - composition over inheritance.
template <typename T>
concept Allocator = requires(T& alloc, size_t size, void* ptr) {
    { alloc.allocate(size) } -> std::convertible_to<void*>;
    { alloc.deallocate(size, ptr) } -> std::convertible_to<void>;
    { alloc.owns(ptr) } -> std::convertible_to<bool>;
};