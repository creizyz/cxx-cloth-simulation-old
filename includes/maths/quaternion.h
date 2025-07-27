#pragma once

#include "common.h"

namespace math
{
    struct quat
    {
        float w, x, y, z;

        quat();
        quat(float f1, float f2, float f3, float f4);
        quat(float t[4]);
        quat(float f, math::vec3 v);

        quat operator-() const;
        quat operator*(float s) const;
        quat operator*(const quat& q2) const;
        quat operator+(const quat& q2) const;

        bool operator==(const quat& q2) const;
        bool operator!=(const quat& q2) const;

        /*quat conjugate() const;
        quat reciprocal() const;
        float unsqrt_norm() const;
        float norm() const;
        bool is_unit() const;*/

        static quat R_quat(float radian, const vec3& axis);
        static quat extractQuat(const mat& m);
        static quat slerp(const quat& q1, const quat& q2, float alpha);
    };
}