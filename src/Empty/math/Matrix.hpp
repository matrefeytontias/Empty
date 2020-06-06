#pragma once

#include "Empty/math/Vector.hpp"

namespace math
{
    template <unsigned int n, typename T>
    struct Matrix
    {
        // Constructors
        Matrix() {}
        static Matrix Constant(T v)
        {
            Matrix r;
            for (int k = 0; k < n * n; k++)
                r._data[k] = v;
            return r;
        }
        // Access
        T operator[](int k) const { return _data[k]; }
        T& operator[](int k) { return _data[k]; }
        T operator()(int i, int j) const { return _data[i * n + j]; }
        T& operator()(int i, int j) { return _data[i * n + j]; }
        operator const T* () const { return _data; }
        operator T* () { return _data; }
        // Infix operators
        Matrix operator+(const Matrix& a) const
        {
            Matrix r;
            for (int k = 0; k < n * n; k++)
                r[k] = _data[k] + a[k];
            return r;
        }
        Matrix operator-(const Matrix& a) const
        {
            Matrix r;
            for (int k = 0; k < n * n; k++)
                r[k] = _data[k] - a[k];
            return r;
        }
        Matrix operator*(T v) const
        {
            Matrix r;
            for (int k = 0; k < n * n; k++)
                r[k] = _data[k] * v;
            return r;
        }
        Matrix operator/(T v) const
        {
            Matrix r;
            for (int k = 0; k < n * n; k++)
                r[k] = _data[k] / v;
            return r;
        }
        Matrix operator*(const Matrix& a) const
        {
            Matrix r;
            const Matrix& me = *this;
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    r(i, j) = 0;
                    for (int k = 0; k < n; k++)
                        r(i, j) += me(i, k) * a(k, j);
                }
            }
            return r;
        }
        // Assignment operators
        Matrix& operator+=(const Matrix& a)
        {
            for (int k = 0; k < n * n; k++)
                _data[k] += a[k];
            return *this;
        }
        Matrix& operator-=(const Matrix& a)
        {
            for (int k = 0; k < n * n; k++)
                _data[k] -= a[k];
            return *this;
        }
        Matrix& operator*=(T v)
        {
            for (int k = 0; k < n * n; k++)
                _data[k] *= v;
            return *this;
        }
        Matrix& operator/=(T v)
        {
            for (int k = 0; k < n * n; k++)
                _data[k] /= v;
            return *this;
        }
        Matrix& operator*=(const Matrix& a)
        {
            return this = this * a;
        }
    protected:
        T _data[n * n];
    };

    using Matrix2f = Matrix<2, float>;
    using Matrix3f = Matrix<3, float>;
    using Matrix4f = Matrix<4, float>;

    using Matrix2d = Matrix<2, double>;
    using Matrix3d = Matrix<3, double>;
    using Matrix4d = Matrix<4, double>;
}
