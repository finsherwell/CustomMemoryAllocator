#pragma once

#include <cstddef>
#include <atomic>

#include "constants.h"
#include "utils.h"

template <size_t BlockSize, size_t BlockCount>
class StaticPoolAllocator
{
private:
    alignas(Constants::CACHE_LINE_SIZE) static std::byte buffer_[BlockSize * BlockCount];
    alignas(Constants::CACHE_LINE_SIZE) std::atomic<std::byte*> free_head_;
    static constexpr size_t ALIGNED_BLOCK_SIZE = Utils::Memory::align_up<Constants::STANDARD_ALIGNMENT>(BlockSize);
    
    static bool initialised_;

    static void initialise() {
        for (size_t i = 0; i < BlockCount - 1; ++i) {
            std::byte* block = buffer_ + i * ALIGNED_BLOCK_SIZE;
            *reinterpret_cast<std::byte**>(block) = buffer_ + (i + 1) * ALIGNED_BLOCK_SIZE;
        }

        // Last block points to null
        std::byte* last_block = buffer_ + (BlockCount - 1) * ALIGNED_BLOCK_SIZE;
        *reinterpret_cast<std::byte**>(last_block) = nullptr;
        
        // This is safe - we know only one thread will initialise
        static_cast<StaticPoolAllocator*>(nullptr)->free_head_.store(buffer_, std::memory_order_relaxed);
    }
public:
    StaticPoolAllocator() {
        if (!initialised_) {
            initialise();
            initialised_ = true;
        }
    }

    void* allocate(size_t size) {
        if (size > BlockSize) return nullptr;
        
        std::byte* head = free_head_.load(std::memory_order_relaxed);
        std::byte* next;
        
        do {
            if (!head) return nullptr; // No free blocks
            next = *reinterpret_cast<std::byte**>(head);
        } while (!free_head_.compare_exchange_weak(head, next, 
                                                  std::memory_order_relaxed));
        
        return head;
    }
    
    void deallocate(void* ptr, size_t size) {
        if (!ptr || !owns(ptr)) return;
        
        std::byte* block = static_cast<std::byte*>(ptr);
        std::byte* head = free_head_.load(std::memory_order_relaxed);
        
        do {
            *reinterpret_cast<std::byte**>(block) = head;
        } while (!free_head_.compare_exchange_weak(head, block, 
                                                  std::memory_order_relaxed));
    }

    bool owns(void* ptr) const {
        return ptr >= buffer_ && ptr < buffer_ + sizeof(buffer_);
    }

    size_t used() const { offset_.load(std::memory_order_relaxed); }
    size_t capacity() const { return Size; }
};

// Static buffer definitions
template<size_t BlockSize, size_t BlockCount>
alignas(Constants::CACHE_LINE_SIZE) std::byte StaticPoolAllocator<BlockSize, BlockCount>::buffer_[BlockSize * BlockCount];

template<size_t BlockSize, size_t BlockCount>
bool StaticPoolAllocator<BlockSize, BlockCount>::initialised_ = false;