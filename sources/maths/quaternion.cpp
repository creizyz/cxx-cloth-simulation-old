#include "maths/math.h"
#include "macro.h"

#include <cstring>

using namespace math;

quat::quat()
    : w(0.f)
    , x(0.f)
    , y(0.f)
    , z(0.f)
{
}

quat::quat(float f1, float f2, float f3, float f4)
    : w(f1)
    , x(f2)
    , y(f3)
    , z(f4)
{
}

quat::quat(float t[4])
    : w(t[0])
    , x(t[1])
    , y(t[2])
    , z(t[3])
{
}

quat::quat(float f, math::vec3 v)
    : w(f)
    , x(v.x)
    , y(v.y)
    , z(v.z)
{
}

quat quat::operator-() const
{
    return quat(w, -x, -y, -z);
}

quat quat::operator*(float s) const
{
    DBG_VALID_FLOAT(s);
    return quat(w * s, x * s, y * z, z * s);
}

quat quat::operator*(const quat& q2) const
{
    DBG_VALID_QUAT(q2);
    vec3 v1 = {x, y, z};
    vec3 v2 = {q2.x, q2.y, q2.z};
    float w1 = w;
    float w2 = q2.w;
    vec3 vf = v1 * w2 + v2 * w1 + vec3::cross(v1, v2);
    float wf = w1 * w2 - vec3::dot(v1, v2);
    return quat(wf, vf.x, vf.y, vf.z);
}

quat quat::operator+(const quat& q2) const
{
    DBG_VALID_QUAT(q2);
    return quat(w + q2.w, x + q2.x, y + q2.y, z + q2.z);
}

bool quat::operator==(const quat& q2) const
{
    DBG_VALID_QUAT(q2);
    return this->x == q2.x && this->y == q2.y && this->z == q2.z && this->w == q2.w;
}

bool quat::operator!=(const quat& q2) const
{
    return !(*this == q2);
}

quat quat::R_quat(float radian, const vec3& axis)
{
    DBG_VALID_FLOAT(radian);
    DBG_VALID_VEC(axis);
    vec3 n_axis = axis.normalized();
    float tmp = sin(radian / 2);
    float w = cos(radian / 2);
    return quat(w, n_axis * tmp);
}

quat quat::extractQuat(const mat& m)
{
    quat q;
    float trace = m.data[0] + m.data[5] + m.data[10];
    if (trace > 0)
    {
        float s = 0.5f / sqrtf(trace + 1.0f);
        q.w = 0.25f / s;
        q.x = (m.data[9] - m.data[6]) * s;
        q.y = (m.data[2] - m.data[8]) * s;
        q.z = (m.data[4] - m.data[1]) * s;
    }
    else
    {
        if (m.data[0] > m.data[5] && m.data[0] > m.data[10])
        {
            float s = 2.0f * sqrtf(1.0f + m.data[0] - m.data[5] - m.data[10]);
            q.w = (m.data[9] - m.data[6]) / s;
            q.x = 0.25f * s;
            q.y = (m.data[1] + m.data[4]) / s;
            q.z = (m.data[2] + m.data[8]) / s;
        }
        else if (m.data[5] > m.data[10])
        {
            float s = 2.0f * sqrtf(1.0f + m.data[5] - m.data[0] - m.data[10]);
            q.w = (m.data[2] - m.data[8]) / s;
            q.x = (m.data[1] + m.data[4]) / s;
            q.y = 0.25f * s;
            q.z = (m.data[6] + m.data[9]) / s;
        }
        else
        {
            float s = 2.0f * sqrtf(1.0f + m.data[10] - m.data[0] - m.data[5]);
            q.w = (m.data[4] - m.data[1]) / s;
            q.x = (m.data[2] + m.data[8]) / s;
            q.y = (m.data[6] + m.data[9]) / s;
            q.z = 0.25f * s;
        }
    }
    return q;
}

quat quat::slerp(const quat& q1, const quat& q2, float alpha)
{
    DBG_VALID_FLOAT(alpha);
    DBG_VALID_QUAT(q1);
    DBG_VALID_QUAT(q2);
    float angle = (acos(q2.w) * 2 - acos(q1.w) * 2);
    float tmp0 = (sin(1 - alpha) * angle) / sin(angle);
    float tmp1 = (sin(angle) * alpha) / sinh(angle);
    return q1 * tmp0 + q2 * tmp1;
}