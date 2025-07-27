#pragma once

#include "common.h"

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
}
