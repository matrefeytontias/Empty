#pragma once

#include "Empty/math/Vector.hpp"

namespace math
{
    /**
     * Template class for matrices. Storage is column-major, in compliance with OpenGL requirements.
     */
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
        static Matrix Identity()
        {
            Matrix r;
            for (int k = 0; k < n * n; k++)
                r._data[k] = !(k % (n + 1));
            return r;
        }
        // Access
        T operator[](int k) const { return _data[k]; }
        T& operator[](int k) { return _data[k]; }
        T operator()(int i, int j) const { return _data[i + j * n]; }
        T& operator()(int i, int j) { return _data[i + j * n]; }
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

    template <unsigned int n, typename T>
    std::ostream& operator<<(std::ostream& s, const Matrix<n, T>& m)
    {
        for (int i = 0; i < n; i++)
        {
            s << "(";
            for (int j = 0; j < n; j++)
                s << m(i, j) << (j == n - 1 ? ")" : ", ");
            s << std::endl;
        }
        return s;
    }

    using mat2 = Matrix<2, float>;
    using mat3 = Matrix<3, float>;
    using mat4 = Matrix<4, float>;

    using dmat2 = Matrix<2, double>;
    using dmat3 = Matrix<3, double>;
    using dmat4 = Matrix<4, double>;
}
