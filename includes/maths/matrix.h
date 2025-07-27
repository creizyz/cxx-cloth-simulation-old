#pragma once

#include "common.h"
#include <vector>

namespace math
{
    struct mat
    {
        float* data;
        int nbrOfRow;
        int nbrOfCol;

        mat();
        mat(int row, int col, float scalar);
        mat(int row, int col);
        mat(int row, int col, float tab[]);
        mat(const vec3& v);
        mat(const mat& m);
        ~mat();

        float* operator[](int row);
        float* operator[](int row) const;
        mat operator*(float scalar) const;
        mat operator/(float scalar) const;
        void operator*=(float scalar);
        void operator/=(float scalar);
        mat operator+(const mat& m) const;
        mat operator-(const mat& m) const;
        void operator+=(const mat& m) const;
        void operator-=(const mat& m);
        mat operator*(const mat& m) const;
        void operator*=(const mat& m);
        mat& operator=(const mat& m);
        bool operator==(const mat& m);
        bool operator!=(const mat& m);

        mat inverse() const;

        mat pinverse() const
        {
            const math::mat& m = *this;
            math::mat mT = math::mat::transpose(m);
            math::mat tmp = m * mT;
            tmp += math::mat(tmp.nbrOfRow, tmp.nbrOfCol, 0.000001f); // <- avoid 0 in the diagonal
            return mT * tmp.inverse();
        }

        static mat inverse(const mat& m);

        static mat pinverse(const mat& m)
        {
            return m.pinverse();
        }

        vec3 operator*(const vec3& v);

        mat transpose() const;
        static mat transpose(const mat& m);

        static mat R_Transform(const quat& q);
        static mat R_Transform(float angle, const math::vec3& axis);
        static mat T_Transform(const vec3& t);
        static mat S_Transform(const vec3& s);

    private:
        void multiplyRow(int row, float scalar);
        void swapRows(int row1, int row2);
        void addRows(int row1, int row2);
        void multipleRowAddition(int row1, int row2, float nbrOfAdditions);
        void substractRows(int row1, int row2);
        void multipleRowSubstraction(int row1, int row2, float nbrOfSubstractions);
    };

    extern vec3 operator*(const mat& m, const vec3& v);

    extern mat triangleProjectionMatrix(const math::vec3& x1, const math::vec3& x2, const math::vec3& x3);
}
