#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <atomic>
#include <memory>
#include <sys/mman.h>

#include "constants.h"

template <size_t Size>
class LinearAllocator
{
private:
    alignas(Constants::CACHE_LINE_SIZE) std::byte* buffer_[Size];
    alignas(Constants::CACHE_LINE_SIZE) std::atomic<size_t> offset_;
    size_t capacity_;
public:
    LinearAllocator(size_t capacity)
    : buffer_(static_cast<std::byte*>(std::aligned_alloc(Constants::CACHE_LINE_SIZE, capacity))),
    offset_(0),
    capacity_(capacity) {
        if (!buffer_) throw std::bad_alloc();

        // Lock pages in memory to avoid page faults

    }

    ~LinearAllocator() {

    }

    void* allocate() {

    }

    void deallocate() {
        // Linear allocator doesn't support individual deallocation
    }

    void reset() {
        offset_.store(0, std::memory_order_relaxed);
    }

    bool owns(const void* ptr) const {
        return ptr >= buffer_ && ptr < buffer_ + capacity_;
    }

    size_t used() { return offset_.load(std::memory_order_relaxed); }
    size_t available() { return capacity_ - used(); }
};