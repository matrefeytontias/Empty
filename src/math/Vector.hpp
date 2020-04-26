#pragma once

#include <memory>

namespace math
{
    template <unsigned int n, typename T = float>
    struct Vector
    {
        // Access
        T operator()(int k) const { return _data[k]; }
        T& operator()(int k) { return _data[k]; }
        T operator[](int k) const { return _data[k]; }
        T& operator[](int k) { return _data[k]; }
        operator const T* () const { return _data; }
        operator T* () { return _data; }
        // Infix operators
        Vector operator+(const Vector& a) const
        {
            Vector r;
            for (int k = 0; k < n; k++)
                r(k) = _data[k] + a(k);
            return r;
        }
        Vector operator-(const Vector& a) const
        {
            Vector r;
            for (int k = 0; k < n; k++)
                r(k) = _data[k] - a(k);
            return r;
        }
        Vector operator*(T v) const
        {
            Vector r;
            for (int k = 0; k < n; k++)
                r(k) = _data[k] * v;
            return r;
        }
        Vector operator/(T v) const
        {
            Vector r;
            for (int k = 0; k < n; k++)
                r(k) = _data[k] / v;
            return r;
        }
        // Assignment operators
        Vector& operator+=(const Vector& a)
        {
            for (int k = 0; k < n; k++)
                _data[k] += a(k);
            return *this;
        }
        Vector& operator-=(const Vector& a)
        {
            for (int k = 0; k < n; k++)
                _data[k] -= a(k);
            return *this;
        }
        Vector& operator*=(T v)
        {
            for (int k = 0; k < n; k++)
                _data[k] *= v;
            return *this;
        }
        Vector& operator/=(T v)
        {
            for (int k = 0; k < n; k++)
                _data[k] /= v;
            return *this;
        }
        // More math
        T dot(const Vector& a) const
        {
            T r = _data[0] * a(0);
            for (int k = 1; k < n; k++)
                r += _data[k] * a(k);
            return r;
        }
        T squaredNorm() const { return dot(*this); }
        T norm() const { return std::sqrt(squaredNorm()); }
        void normalize() { *this /= norm(); }
        Vector normalized() const { return *this / norm(); }

    private:
        T _data[n];
    };

    //
    // Template specializations for few-element vectors
    //
    template <typename T> struct Vector2 : public Vector<2, T>
    {
        Vector2(T x, T y)
        {
            _data[0] = x;
            _data[1] = y;
        }
        Vector2(const Vector<2, T>& a) { std::memcpy(_data, a._data, sizeof(_data)); }

        T x() const { return _data[0]; }
        T& x() { return _data[0]; }
        T y() const { return _data[1]; }
        T& y() { return _data[1]; }
    };

    template <typename T> struct Vector3 : public Vector<3, T>
    {
        Vector3(T x, T y, T z)
        {
            _data[0] = x;
            _data[1] = y;
            _data[2] = z;
        }
        Vector3(const Vector<3, T>& a) { std::memcpy(_data, a._data, sizeof(_data)); }

        T x() const { return _data[0]; }
        T& x() { return _data[0]; }
        T y() const { return _data[1]; }
        T& y() { return _data[1]; }
        T z() const { return _data[2]; }
        T& z() { return _data[2]; }

        Vector3 cross(const Vector3& a) const
        {
            Vector3 r;
            r(0) = _data[1] * a(2) - _data[2] * a(1);
            r(1) = _data[2] * a(0) - _data[0] * a(2);
            r(2) = _data[0] * a(1) - _data[1] * a(0);
            return r;
        }
    };

    template <typename T> struct Vector4 : public Vector<4, T>
    {
        Vector4(T x, T y, T z, T w)
        {
            _data[0] = x;
            _data[1] = y;
            _data[2] = z;
            _data[3] = w;
        }
        Vector4(const Vector<4, T>& a) { std::memcpy(_data, a._data, sizeof(_data)); }

        T x() const { return _data[0]; }
        T& x() { return _data[0]; }
        T y() const { return _data[1]; }
        T& y() { return _data[1]; }
        T z() const { return _data[2]; }
        T& z() { return _data[2]; }
        T w() const { return _data[3]; }
        T& w() { return _data[3]; }
    };

    //
    // Few-element aliases
    //

    using Vector2f = Vector2<float>;
    using Vector3f = Vector3<float>;
    using Vector4f = Vector4<float>;

    using Vector2d = Vector2<double>;
    using Vector3d = Vector3<double>;
    using Vector4d = Vector4<double>;
}
