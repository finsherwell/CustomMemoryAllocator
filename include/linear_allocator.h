#pragma once

#include <cstdlib>
#include <atomic>
#include <memory>

class LinearAllocator
{
private:
    std::size_t total_size;
    std::atomic<std::unique_ptr<std::size_t>> ptr;
public:
    LinearAllocator(const std::size_t buffer_size);
    ~LinearAllocator();
    void allocate(const std::size_t size, const std::size_t alignment = 0);
    void free();
    void reset();
};
