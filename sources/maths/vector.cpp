#include "maths/math.h"
#include "macro.h"

#include <cstring>

using namespace math;

vec3::vec3()
: x(0.f)
, y(0.f)
, z(0.f)
{
}

vec3::vec3(float x, float y, float z)
    : x(x)
    , y(y)
    , z(z)
{
}

void vec3::operator=(const vec3& v2)
{
    DBG_VALID_VEC(v2);
    x = v2.x;
    y = v2.y;
    z = v2.z;
}

void vec3::operator+=(const vec3& v2)
{
    DBG_VALID_VEC(v2);
    x += v2.x;
    y += v2.y;
    z += v2.z;
}

bool vec3::operator==(const vec3& v2) const
{
    DBG_VALID_VEC(v2);
    return x == v2.x && y == v2.y && z == v2.z;
}

bool vec3::operator!=(const vec3& v2) const
{
    DBG_VALID_VEC(v2);
    return !operator==(v2);
}

vec3 vec3::operator-(const vec3& v2) const
{
    DBG_VALID_VEC(v2);
    return vec3(x - v2.x, y - v2.y, z - v2.z);
}

vec3 vec3::operator+(const vec3& v2) const
{
    DBG_VALID_VEC(v2);
    return vec3(x + v2.x, y + v2.y, z + v2.z);
}

vec3 vec3::operator*(float scalar) const
{
    DBG_VALID_FLOAT(scalar);
    return vec3(x * scalar, y * scalar, z * scalar);
}

vec3 vec3::operator/(const float scalar) const
{
    DBG_VALID_FLOAT(scalar);
    return vec3(x / scalar, y / scalar, z / scalar);
}

vec3 vec3::operator-() const
{
    return vec3(-x, -y, -z);
}

float vec3::length() const
{
    return sqrt(x * x + y * y + z * z);
}

float vec3::unsqrt_length() const
{
    return (x * x + y * y + z * z);
}

vec3 vec3::normalized() const
{
    DBG_ASSERT(length() != (0.f));
    return *this / length();
}

float vec3::dot(const vec3& v1, const vec3& v2)
{
    DBG_VALID_VEC(v1);
    DBG_VALID_VEC(v2);
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3 vec3::cross(const vec3& v1, const vec3& v2)
{
    DBG_VALID_VEC(v1);
    DBG_VALID_VEC(v2);
    float x = v1.y * v2.z - v2.y * v1.z;
    float y = v1.z * v2.x - v2.z * v1.x;
    float z = v1.x * v2.y - v2.x * v1.y;
    return vec3(x, y, z);
}

vec3 math::operator*(const float scalar, const vec3& v)
{
    return v * scalar;
}