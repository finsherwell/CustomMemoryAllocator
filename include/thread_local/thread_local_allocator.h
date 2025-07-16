#pragma once

#include <cstddef>
#include <atomic>

#include "constants.h"
#include "utils.h"

template <size_t Size>
class ThreadLocalAllocator
{
private:
    alignas(Constants::CACHE_LINE_SIZE) thread_local static std::byte buffer_[Size];
    alignas(Constants::CACHE_LINE_SIZE) thread_local static size_t offset_;
public:
    void* allocate(size_t size, size_t alignment = alignment(std::max_align_t)) {
        size_t aligned_size = Utils::Memory::align_up<Constants::STANDARD_ALIGNMENT>(size);

        size_t aligned_offset = Utils::Memory::align_up<Constants::STANDARD_ALIGNMENT>(offset_);
        size_t new_offset = aligned_offset + aligned_size;

        if (new_offset > size) {
            return nullptr;
        }

        offset_ = new_offset;
        return buffer_ + aligned_offset;
    }

    void deallocate() {
        // Linear allocator doesn't support individual deallocation
    }

    void reset() {
        offset_ = 0;
    }

    bool owns(void* ptr) const {
        return ptr >= buffer_ && ptr < buffer_ + Size;
    }

    size_t used() const { return offset_; }
    constexpr size_t capacity const { return Size; }
};

// Thread-local static definitions
template <size_t Size>
alignas(Constants::CACHE_LINE_SIZE) thread_local std::byte ThreadLocalAllocator<Size>::buffer_[Size];

template <size_t Size>
thread_local size_t ThreadLocalAllocator<Size>::offset_ = 0;