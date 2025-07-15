#pragma once

#include <cstddef>
#include <atomic>

#include "constants.h"
#include "utils.h"

template <size_t Size>
class StaticLinearAllocator
{
private:
    alignas(Constants::CACHE_LINE_SIZE) static std::byte buffer_[Size];
    alignas(Constants::CACHE_LINE_SIZE) std::atomic<size_t> offset_;
public:
    StaticLinearAllocator() : offset_(0) {}
    
    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
        size_t aligned_size = Utils::Memory::align_up<alignment>(size);

        size_t current_offset = offset_.load(std::memory_order_relaxed);
        size_t new_offset;

        do {
            size_t aligned_offset = Utils::Memory::align_up<alignment>(current_offset);
            new_offset = aligned_offset + aligned_size;

            if (new_offset > Size) {
                // Out of memory
                return nullptr;
            }
        } while (!offset_.compare_exchange_weak(current_offset, new_offset, std::memory_order_relaxed));

        return buffer_ + Utils::Memory::align_up<alignment>(current_offset);
    }

    void deallocate(size_t size, void* ptr) {
        // Linear allocator doesn't support individual deallocation
    }

    void reset() {
        offset_.store(0, std::memory_order_relaxed);
    }

    bool owns(const void* ptr) const {
        return ptr >= buffer_ && ptr < buffer_ + Size;
    }

    size_t used() const { return offset_.load(std::memory_order_relaxed); }
    constexpr size_t capacity() const { return Size; }
};

// Static buffer definition
template <size_t Size>
alignas(Constants::CACHE_LINE_SIZE) std::byte StaticLinearAllocator<Size>::buffer_[Size];