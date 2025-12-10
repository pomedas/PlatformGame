#include <cstdlib>
#include <new>
#include "tracy/Tracy.hpp"

// Global operator new/delete so Tracy can track all allocations.

void* operator new(std::size_t count)
{
    if (count == 0) count = 1;
    void* ptr = std::malloc(count);
    if (!ptr) throw std::bad_alloc();

    TracyAlloc(ptr, count);
    return ptr;
}

void operator delete(void* ptr) noexcept
{
    if (!ptr) return;
    TracyFree(ptr);
    std::free(ptr);
}

void* operator new[](std::size_t count)
{
    if (count == 0) count = 1;
    void* ptr = std::malloc(count);
    if (!ptr) throw std::bad_alloc();

    TracyAlloc(ptr, count);
    return ptr;
}

void operator delete[](void* ptr) noexcept
{
    if (!ptr) return;
    TracyFree(ptr);
    std::free(ptr);
}

#if __cpp_sized_deallocation
void operator delete(void* ptr, std::size_t) noexcept
{
    if (!ptr) return;
    TracyFree(ptr);
    std::free(ptr);
}

void operator delete[](void* ptr, std::size_t) noexcept
{
    if (!ptr) return;
    TracyFree(ptr);
    std::free(ptr);
}
#endif
