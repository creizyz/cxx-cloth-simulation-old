#pragma once

#include <immintrin.h>
#include <cstdlib>
#include <memory>

template <typename RegisterType>
struct SIMD;

template <>
struct SIMD<__m128>
{
    static constexpr size_t width = 4;
    static constexpr size_t alignment = 16;
};

template <>
struct SIMD<__m256>
{
    static constexpr size_t width = 8;
    static constexpr size_t alignment = 32;
};

template <>
struct SIMD<__m512>
{
    static constexpr size_t width = 16;
    static constexpr size_t alignment = 64;
};

enum class SIMD_TYPE
{
    NONE,
    SIMD_128,
    SIMD_256,
    SIMD_512
};

constexpr SIMD_TYPE get_simd_type()
{
#if defined(__AVX512F__)
    return SIMD_TYPE::SIMD_512;
#else if defined(__AVX2__)
    return SIMD_TYPE::SIMD_256;
#else if defined(__AVX__)
    return SIMD_TYPE::SIMD_128;
#else
    return SIMD_TYPE::NONE;
#endif
}

template <typename T>
using aligned_unique_ptr = std::unique_ptr<T, decltype(&std::free)>;

template <typename T>
auto make_aligned_unique(size_t count, size_t alignment)
{
    // Check if T is an array type
    if constexpr (std::is_array_v<T>)
    {
        using U = std::remove_extent_t<T>; // Remove the [] part
        void* ptr = std::aligned_alloc(alignment, count * sizeof(U));
        if (!ptr)
        {
            throw std::bad_alloc();
        }
        return aligned_unique_ptr<T>(static_cast<U*>(ptr), std::free);
    }
    else
    {
        static_assert(sizeof(T) >= alignment, "T must be large enough for given alignment.");
        void* ptr = std::aligned_alloc(alignment, count * sizeof(T));
        if (!ptr)
        {
            throw std::bad_alloc();
        }
        return aligned_unique_ptr<T>(static_cast<T*>(ptr), std::free);
    }
}

