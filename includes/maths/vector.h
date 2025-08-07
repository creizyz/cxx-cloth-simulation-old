#pragma once

#include <memory>

#include "common.h"
#include "simd.h"

namespace math
{
    struct vec3
    {
        vec3();
        vec3(float x, float y, float z);
        float x, y, z;

        bool operator==(const vec3& v2) const;
        bool operator!=(const vec3& v2) const;

        void operator=(const vec3& v2);
        void operator+=(const vec3& v2);

        vec3 operator-(const vec3& v2) const;
        vec3 operator+(const vec3& v2) const;
        vec3 operator*(const float scalar) const;
        vec3 operator/(const float scalar) const;
        vec3 operator-() const;

        float length() const;
        float unsqrt_length() const;
        vec3 normalized() const;

        static float dot(const vec3& v1, const vec3& v2);
        static vec3 cross(const vec3& v1, const vec3& v2);

        /*
        static vec3 getTranslation(const Matrix4f & m) { return Vector3(m[0][3], m[1][3], m[2][3]); }
        static vec3 getRotationAxis(const quat & q) { return vec3(q.x, q.y, q.z).normalized(); }
        static vec3 getEulerAngles(const quat & q) { return vec3(atan2(2 * (q.w*q.x + q.y*q.z), 1 - (2 * (q.x*q.x + q.y*q.y))), asin(2 * (q.w*q.y - q.z*q.x)), atan2(2 * (q.w*q.z + q.x*q.y), 1 - (2 * (q.y*q.y + q.z*q.z)))); }
        */
    };

    extern vec3 operator*(const float scalar, const vec3& v);


    template <typename RegisterType>
    class Vec3D_simd
    {
    private:
        constexpr static size_t width     = SIMD<RegisterType>::width;
        constexpr static size_t alignment = SIMD<RegisterType>::alignment;

        aligned_unique_ptr<float[]> _x{ nullptr, std::free };
        aligned_unique_ptr<float[]> _y{ nullptr, std::free };
        aligned_unique_ptr<float[]> _z{ nullptr, std::free };

        size_t _count{ 0 };

    public:
        Vec3D_simd()
        { }

        explicit Vec3D_simd(size_t count)
            : _x{ make_aligned_unique<float[]>(count, alignment) }
            , _y{ make_aligned_unique<float[]>(count, alignment) }
            , _z{ make_aligned_unique<float[]>(count, alignment) }
            , _count{ ((count + width - 1) / width) * width } // round to ceiling width
        { }

        float & x(size_t index) const
        {
            return x[index];
        }

        float & y(size_t index) const
        {
            return x[index];
        }

        float & z(size_t index) const
        {
            return x[index];
        }

        size_t size() const
        {
            return _count;
        }
    };

    template <typename RegisterType>
    void add_3D_vectors_simd(const Vec3D_simd<RegisterType> & a, const Vec3D_simd<RegisterType> & b, Vec3D_simd<RegisterType> & result);

    template <>
    void add_3D_vectors_simd<__m128>(const Vec3D_simd<__m128> & a, const Vec3D_simd<__m128> & b, Vec3D_simd<__m128> & result)
    {
        constexpr auto width = SIMD<__m128>::width;
        const auto count = std::min(a.size(), b.size(), result.size());
        for (auto i = 0u; i < count; i += width)
        {
            __m128 ax = _mm_load_ps(&a.x(i));
            __m128 ay = _mm_load_ps(&a.y(i));
            __m128 az = _mm_load_ps(&a.z(i));

            __m128 bx = _mm_load_ps(&b.x(i));
            __m128 by = _mm_load_ps(&b.y(i));
            __m128 bz = _mm_load_ps(&b.z(i));

            __m128 rx = _mm_add_ps(ax, bx);
            __m128 ry = _mm_add_ps(ay, by);
            __m128 rz = _mm_add_ps(az, bz);

            _mm_store_ps(&result.x(i), rx);
            _mm_store_ps(&result.y(i), ry);
            _mm_store_ps(&result.z(i), rz);
        }
    }

    template <>
    void add_3D_vectors_simd<__m256>(const Vec3D_simd<__m256> & a, const Vec3D_simd<__m256> & b, Vec3D_simd<__m256> & result)
    {
        constexpr auto width = SIMD<__m256>::width;
        const auto count = std::min(a.size(), b.size(), result.size());
        for (auto i = 0u; i < count; i += width)
        {
            __m256 ax = _mm256_load_ps(&a.x(i));
            __m256 ay = _mm256_load_ps(&a.y(i));
            __m256 az = _mm256_load_ps(&a.z(i));

            __m256 bx = _mm256_load_ps(&b.x(i));
            __m256 by = _mm256_load_ps(&b.y(i));
            __m256 bz = _mm256_load_ps(&b.z(i));

            __m256 rx = _mm256_add_ps(ax, bx);
            __m256 ry = _mm256_add_ps(ay, by);
            __m256 rz = _mm256_add_ps(az, bz);

            _mm256_store_ps(&result.x(i), rx);
            _mm256_store_ps(&result.y(i), ry);
            _mm256_store_ps(&result.z(i), rz);
        }
    }

    template <>
    void add_3D_vectors_simd<__m512>(const Vec3D_simd<__m512> & a, const Vec3D_simd<__m512> & b, Vec3D_simd<__m512> & result)
    {
        constexpr auto width = SIMD<__m512>::width;
        const auto count = std::min(a.size(), b.size(), result.size());
        for (auto i = 0u; i < count; i += width)
        {
            __m512 ax = _mm512_load_ps(&a.x(i));
            __m512 ay = _mm512_load_ps(&a.y(i));
            __m512 az = _mm512_load_ps(&a.z(i));

            __m512 bx = _mm512_load_ps(&b.x(i));
            __m512 by = _mm512_load_ps(&b.y(i));
            __m512 bz = _mm512_load_ps(&b.z(i));

            __m512 rx = _mm512_add_ps(ax, bx);
            __m512 ry = _mm512_add_ps(ay, by);
            __m512 rz = _mm512_add_ps(az, bz);

            _mm512_store_ps(&result.x(i), rx);
            _mm512_store_ps(&result.y(i), ry);
            _mm512_store_ps(&result.z(i), rz);
        }
    }
}
