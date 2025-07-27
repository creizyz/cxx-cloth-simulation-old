#include "maths/math.h"
#include "macro.h"

#include <cstring>
#include <exception>

using namespace math;

mat::mat()
    : data(nullptr)
    , nbrOfRow(0)
    , nbrOfCol(0)
{
}

mat::mat(int row, int col, float scalar)
    : mat()
{
    try
    {
        nbrOfRow = row;
        nbrOfCol = col;
        data = new float[nbrOfRow * nbrOfCol];
        for (int row = 0; row < nbrOfRow; row++)
            for (int col = 0; col < nbrOfCol; col++)
                if (row == col) (*this)[row][col] = scalar;
                else (*this)[row][col] = 0.f;
    }
    catch (const std::bad_alloc&)
    {
        nbrOfRow = 0;
        nbrOfCol = 0;
        SAFE_DELETE_TAB(data);
        data = nullptr;
    }
}

mat::mat(int row, int col)
    : mat(row, col, 1.f)
{
}

mat::mat(int row, int col, float tab[])
    : mat(row, col)
{
    for (int n = 0; n < nbrOfRow * nbrOfCol; n++)
    {
        DBG_VALID_FLOAT(tab[n]);
        data[n] = tab[n];
    }
}

mat::mat(const vec3& v)
    : mat(4, 1)
{
    data[0] = v.x;
    data[1] = v.y;
    data[2] = v.z;
    data[3] = 1.f;
}

mat::mat(const mat& m)
    : mat(m.nbrOfRow, m.nbrOfCol)
{
    for (int n = 0; n < nbrOfRow * nbrOfCol; n++) data[n] = m.data[n];
}

mat::~mat()
{
    SAFE_DELETE_TAB(data);
}

float* mat::operator[](int row)
{
    return (data + (row * nbrOfCol));
}

float* mat::operator[](int row) const
{
    return (data + (row * nbrOfCol));
}

mat mat::operator*(float scalar) const
{
    mat r(nbrOfRow, nbrOfCol);
    for (int n = 0; n < nbrOfRow * nbrOfCol; n++) r.data[n] = data[n] * scalar;
    return r;
}

mat mat::operator/(float scalar) const
{
    mat r(nbrOfRow, nbrOfCol);
    for (int n = 0; n < nbrOfRow * nbrOfCol; n++) r.data[n] = data[n] / scalar;
    return r;
}

void mat::operator*=(float scalar)
{
    for (int n = 0; n < nbrOfRow * nbrOfCol; n++) data[n] *= scalar;
}

void mat::operator/=(float scalar)
{
    for (int n = 0; n < nbrOfRow * nbrOfCol; n++) data[n] /= scalar;
}

mat mat::operator+(const mat& m) const
{
    DBG_VALID_MAT(m);
    DBG_ASSERT(nbrOfRow == m.nbrOfRow && nbrOfCol == m.nbrOfCol);
    mat r(nbrOfRow, nbrOfCol);
    if (nbrOfRow == m.nbrOfRow && nbrOfCol == m.nbrOfCol)
    {
        for (int n = 0; n < nbrOfRow * nbrOfCol; n++) r.data[n] = data[n] + m.data[n];
        return r;
    }
}

mat mat::operator-(const mat& m) const
{
    DBG_VALID_MAT(m);
    DBG_ASSERT(nbrOfRow == m.nbrOfRow && nbrOfCol == m.nbrOfCol);
    mat r(nbrOfRow, nbrOfCol);
    if (nbrOfRow == m.nbrOfRow && nbrOfCol == m.nbrOfCol)
    {
        for (int n = 0; n < nbrOfRow * nbrOfCol; n++) r.data[n] = data[n] - m.data[n];
        return r;
    }
}

void mat::operator+=(const mat& m) const
{
    DBG_VALID_MAT(m);
    DBG_ASSERT(nbrOfRow == m.nbrOfRow && nbrOfCol == m.nbrOfCol);
    if (nbrOfRow == m.nbrOfRow && nbrOfCol == m.nbrOfCol)
        for (int n = 0; n < nbrOfRow * nbrOfCol; n++) data[n] += m.data[n];
}

void mat::operator-=(const mat& m)
{
    DBG_VALID_MAT(m);
    DBG_ASSERT(nbrOfRow == m.nbrOfRow && nbrOfCol == m.nbrOfCol);
    if (nbrOfRow == m.nbrOfRow && nbrOfCol == m.nbrOfCol)
        for (int n = 0; n < nbrOfRow * nbrOfCol; n++) data[n] -= m.data[n];
}

mat mat::operator*(const mat& m) const
{
    DBG_VALID_MAT(m);
    DBG_ASSERT(nbrOfCol == m.nbrOfRow);
    if (nbrOfCol == m.nbrOfRow)
    {
        mat r(nbrOfRow, m.nbrOfCol);
        for (int row = 0; row < nbrOfRow; row++)
        {
            for (int col = 0; col < m.nbrOfCol; col++)
            {
                float newElement = 0.f;
                for (int n = 0; n < nbrOfCol; n++)
                {
                    // float A = (*this)[row][n];
                    // float B = m[n][col];
                    newElement += (*this)[row][n] * m[n][col];
                }
                r[row][col] = newElement;
            }
        }
        return r;
    }
}

void mat::operator*=(const mat& m)
{
    DBG_VALID_MAT(m);
    DBG_ASSERT(nbrOfCol == m.nbrOfRow);
    if (nbrOfCol == m.nbrOfRow)
    {
        (*this) = (*this) * m;
    }
}

mat& mat::operator=(const mat& m)
{
    DBG_VALID_MAT(m);
    SAFE_DELETE_TAB(data);

    nbrOfRow = m.nbrOfRow;
    nbrOfCol = m.nbrOfCol;
    data = new float[nbrOfRow * nbrOfCol];

    for (int n = 0; n < nbrOfRow * nbrOfCol; n++) data[n] = m.data[n];

    return *this;
}

bool mat::operator==(const mat& m)
{
    DBG_VALID_MAT(m);
    DBG_ASSERT(nbrOfRow == m.nbrOfRow && nbrOfCol == m.nbrOfCol);
    if (nbrOfRow == m.nbrOfRow && nbrOfCol == m.nbrOfCol)
    {
        for (int n = 0; n < nbrOfRow * nbrOfCol; n++) if (data[n] <= m.data[n] - MATH_EPSILON && data[n] >= m.data[n] + MATH_EPSILON) return false;
        return true;
    }
    return false;
}

bool mat::operator!=(const mat& m)
{
    DBG_VALID_MAT(m);
    DBG_ASSERT(nbrOfRow == m.nbrOfRow && nbrOfCol == m.nbrOfCol);
    return !((*this) == m);
}

mat mat::inverse() const
{
    if (nbrOfRow == nbrOfCol)
    {
        mat tmp(*this);
        mat res(nbrOfRow, nbrOfCol);

        // Gauss jordan algorithm
        for (int n = 0; n < nbrOfRow; n++)
        {
            if (tmp[n][n] != 1.f && tmp[n][n] != 0.f) // rounding error?
            {
                float ratio = 1 / tmp[n][n];
                tmp.multiplyRow(n, ratio);
                res.multiplyRow(n, ratio);
            }
            for (int row = 0; row < nbrOfRow; row++)
            {
                if (row != n)
                {
                    res.multipleRowSubstraction(row, n, tmp[row][n]);
                    tmp.multipleRowSubstraction(row, n, tmp[row][n]);
                }
            }
        }

        return res;
    }
    else
        return pinverse();
}

mat mat::inverse(const mat& m)
{
    DBG_VALID_MAT(m);
    return m.inverse();
}

mat mat::transpose() const
{
    mat r(nbrOfCol, nbrOfRow);
    for (int row = 0; row < nbrOfRow; row++)
    {
        for (int col = 0; col < nbrOfCol; col++)
        {
            r[col][row] = data[row * nbrOfCol + col];
        }
    }
    return r;
}

mat mat::transpose(const mat& m)
{
    DBG_VALID_MAT(m);
    return m.transpose();
}

void mat::multiplyRow(int row, float scalar)
{
    for (int n = row * nbrOfCol; n < (row + 1) * nbrOfCol; n++) data[n] *= scalar;
}

void mat::swapRows(int row1, int row2)
{
    int row1p = row1 * nbrOfCol;
    int row2p = row2 * nbrOfCol;
    for (int n = 0; n < nbrOfCol; n++)
    {
        float tmp = data[row1p + n];
        data[row1p + n] = data[row2p + n];
        data[row2p + n] = tmp;
    }
}

void mat::addRows(int row1, int row2) // row1 = row1 + row2
{
    int row1p = row1 * nbrOfCol;
    int row2p = row2 * nbrOfCol;
    for (int n = 0; n < nbrOfCol; n++) data[row1p + n] += data[row2p + n];
}

void mat::multipleRowAddition(int row1, int row2, float nbrOfAdditions)
{
    int row1p = row1 * nbrOfCol;
    int row2p = row2 * nbrOfCol;
    for (int n = 0; n < nbrOfCol; n++) data[row1p + n] += nbrOfAdditions * data[row2p + n];
}

void mat::substractRows(int row1, int row2) // row1 = row1 - row2
{
    int row1p = row1 * nbrOfCol;
    int row2p = row2 * nbrOfCol;
    for (int n = 0; n < nbrOfCol; n++) data[row1p + n] -= data[row2p + n];
}

void mat::multipleRowSubstraction(int row1, int row2, float nbrOfSubstractions)
{
    int row1p = row1 * nbrOfCol;
    int row2p = row2 * nbrOfCol;
    for (int n = 0; n < nbrOfCol; n++) data[row1p + n] -= nbrOfSubstractions * data[row2p + n];
}

vec3 mat::operator*(const vec3& v)
{
    DBG_ASSERT(nbrOfRow == 3 || nbrOfRow == 4 && nbrOfCol == 3 || nbrOfCol == 4);
    DBG_VALID_VEC(v);
    float x = (data[0] * v.x + data[1] * v.y + data[2] * v.z) + data[3];
    float y = (data[4] * v.x + data[5] * v.y + data[6] * v.z) + data[7];
    float z = (data[8] * v.x + data[9] * v.y + data[10] * v.z) + data[11];
    if (nbrOfCol == 4)
    {
        x += data[3];
        y += data[7];
        z += data[11];
    }
    return {x, y, z};
}

mat mat::R_Transform(float r_angle, const math::vec3& r_axis)
{
    DBG_VALID_VEC(r_axis);
    DBG_VALID_FLOAT(r_angle);
    float sin_angle = sin(r_angle);
    float cos_angle = cos(r_angle);
    float inv_cos_angle = 1.f - cos_angle;

    math::vec3 axis = r_axis.normalized();

    mat r(4, 4, 1);
    r.data[0] = inv_cos_angle * axis.x * axis.x + cos_angle;
    r.data[1] = inv_cos_angle * axis.x * axis.y - (sin_angle * axis.z);
    r.data[2] = inv_cos_angle * axis.x * axis.z + (sin_angle * axis.y);
    r.data[4] = inv_cos_angle * axis.y * axis.x + (sin_angle * axis.z);
    r.data[5] = inv_cos_angle * axis.y * axis.y + cos_angle;
    r.data[6] = inv_cos_angle * axis.y * axis.z - (sin_angle * axis.x);
    r.data[8] = inv_cos_angle * axis.z * axis.x - (sin_angle * axis.y);
    r.data[9] = inv_cos_angle * axis.z * axis.y + (sin_angle * axis.x);
    r.data[10] = inv_cos_angle * axis.z * axis.z + cos_angle;
    return r;
}

mat mat::R_Transform(const quat& q)
{
    DBG_VALID_QUAT(q);

    float angle = acos(q.w) * 2.f;
    float tmp = sin(angle / 2.f);
    vec3 axis = (vec3(q.x, q.y, q.z) / tmp).normalized();

    // Page 466, Graphics Gems

    float sin_angle = sin(angle);
    float cos_angle = cos(angle);
    float inv_cos_angle = 1.f - cos_angle;

    mat r(4, 4, 1);
    r.data[0] = inv_cos_angle * axis.x * axis.x + cos_angle;
    r.data[1] = inv_cos_angle * axis.x * axis.y - (sin_angle * axis.z);
    r.data[2] = inv_cos_angle * axis.x * axis.z + (sin_angle * axis.y);
    r.data[4] = inv_cos_angle * axis.y * axis.x + (sin_angle * axis.z);
    r.data[5] = inv_cos_angle * axis.y * axis.y + cos_angle;
    r.data[6] = inv_cos_angle * axis.y * axis.z - (sin_angle * axis.x);
    r.data[8] = inv_cos_angle * axis.z * axis.x - (sin_angle * axis.y);
    r.data[9] = inv_cos_angle * axis.z * axis.y + (sin_angle * axis.x);
    r.data[10] = inv_cos_angle * axis.z * axis.z + cos_angle;
    return r;
}

mat mat::T_Transform(const vec3& t)
{
    DBG_VALID_VEC(t);
    mat res(4, 4, 1.f);
    res[0][3] = t.x;
    res[1][3] = t.y;
    res[2][3] = t.z;
    return res;
}

mat mat::S_Transform(const vec3& s)
{
    DBG_VALID_VEC(s);
    mat res(4, 4, 1.f);
    res[0][0] = s.x;
    res[1][1] = s.y;
    res[2][2] = s.z;
    return res;
}


mat math::triangleProjectionMatrix(const math::vec3& x1, const math::vec3& x2, const math::vec3& x3)
{
    DBG_VALID_VEC(x1);
    DBG_VALID_VEC(x2);
    DBG_VALID_VEC(x3);
    math::vec3 tmp1 = x2 - x1;
    math::vec3 tmp2 = x3 - x1;
    math::vec3 tmp3 = math::vec3::cross(tmp1, tmp2);
    math::vec3 planeNormal = math::vec3::cross(x2 - x1, x3 - x1).normalized();
    math::vec3 Ox = (x2 - x1).normalized();
    math::vec3 Oy = math::vec3::cross(planeNormal, Ox).normalized();
    DBG_VALID_VEC(Ox);
    DBG_VALID_VEC(Oy);
    math::mat projectionMatrix(2, 3);
    projectionMatrix[0][0] = Ox.x;
    projectionMatrix[0][1] = Oy.x;
    projectionMatrix[0][2] = Ox.y;
    projectionMatrix[1][0] = Oy.y;
    projectionMatrix[1][1] = Ox.z;
    projectionMatrix[1][2] = Oy.z;
    return projectionMatrix;
}
