#pragma once

#include <iostream>

namespace math
{
    template <typename T> struct _vec2;
    template <typename T> struct _vec3;
    template <typename T> struct _vec4;

// Array accessors and pointer casts are the same for vectors with any amount of components
#define DEFINE_ACCESSORS T operator()(int k) const { return (&x)[k]; } \
    T& operator()(int k) { return (&x)[k]; } \
    T operator[](int k) const { return (&x)[k]; } \
    T& operator[](int k) { return (&x)[k]; } \
    operator const T* () const { return &x; } \
    operator T* () { return &x; }

    /**
    * 2-component vector
    */
    template <typename T>
    struct _vec2
    {
        using ElementType = T;
        union
        {
            struct { T x; T y; };
            struct { T r; T g; };
            struct { T s; T t; };
        };
        
        // Constructors
        _vec2() = default;

        template <typename U>
        _vec2(U x, U y) : x(static_cast<T>(x)), y(static_cast<T>(y)) {}
        
        template <typename U>
        _vec2(const _vec2<U>& v) { this->xy() = v; }
        
        // Access
        DEFINE_ACCESSORS;
        
        // Infix and assignment operators
#define OP(op) inline _vec2 operator##op(const _vec2& v) const { return _vec2(x op v.x, y op v.y); } \
               inline _vec2 operator##op(T v) const { return _vec2(x op v, y op v); } \
               inline _vec2& operator##op##=(const _vec2& v) { x op##= v.x; y op##= v.y; return *this; } \
               inline _vec2& operator##op##=(T v) { x op##= v; y op##= v; return *this; }
        OP(+);
        OP(-);
        OP(*);
        OP(/);
#undef OP

    private:
#include "vecref.inl"
    public:
#include "swizzling.inl"
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& s, const _vec2<T>& v)
    {
        return s << "(" << v.x << ", " << v.y << ")";
    }

    template <typename T>
    struct _vec3
    {
        using ElementType = T;
        union
        {
            struct { T x; T y; T z; };
            struct { T r; T g; T b; };
            struct { T s; T t; T p; };
        };

        // Constructors
        _vec3() = default;

        template <typename U>
        _vec3(U x, U y, U z) : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)) {}
        
        template <typename U>
        _vec3(const _vec2<U>& xy, U z) : z(static_cast<T>(z)) { this->xy() = xy; }
        
        template <typename U>
        _vec3(U x, const _vec2<U>& yz) : x(static_cast<T>(x)) { this->yz() = yz; }
        
        template <typename U>
        _vec3(const _vec3<U>& v) { this->xyz() = v; }
        
        // Access
        DEFINE_ACCESSORS;

        // Infix and assignment operators
#define OP(op) inline _vec3 operator##op(const _vec3& v) const { return _vec3(x op v.x, y op v.y, z op v.z); } \
               inline _vec3 operator##op(T v) const { return _vec3(x op v, y op v, z op v); } \
               inline _vec3& operator##op##=(const _vec3& v) { x op##= v.x; y op##= v.y; z op##= v.z; return *this; } \
               inline _vec3& operator##op##=(T v) { x op##= v; y op##= v; z op##= v; return *this; }
        OP(+);
        OP(-);
        OP(*);
        OP(/);
#undef OP

        static _vec3<T> right;
        static _vec3<T> up;
        static _vec3<T> forward;

    private:
#include "vecref.inl"
    public:
#define VEC_HAS_Z
#include "swizzling.inl"
#undef VEC_HAS_Z
    };

    template <typename T>
    _vec3<T> _vec3<T>::right = _vec3<T>(1, 0, 0);
    template <typename T>
    _vec3<T> _vec3<T>::up = _vec3<T>(0, 1, 0);
    template <typename T>
    _vec3<T> _vec3<T>::forward = _vec3<T>(0, 0, 1);

    template <typename T>
    std::ostream& operator<<(std::ostream& s, const _vec3<T>& v)
    {
        return s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }

    template <typename T>
    struct _vec4
    {
        using ElementType = T;
        union
        {
            struct { T x; T y; T z; T w; };
            struct { T r; T g; T b; T a; };
            struct { T s; T t; T p; T q; };
        };

        // Constructors
        _vec4() = default;

        template <typename U>
        _vec4(U x, U y, U z, U w) : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)), w(static_cast<T>(w)) {}

        template <typename U>
        _vec4(const _vec2<U>& xy, U z, U w) : z(static_cast<T>(z)), w(static_cast<T>(w)) { this->xy() = xy; }

        template <typename U>
        _vec4(U x, const _vec2<U>& yz, U w) : x(static_cast<T>(x)), w(static_cast<T>(w)) { this->yz() = yz; }

        template <typename U>
        _vec4(U x, U y, const _vec2<U>& zw) : x(static_cast<T>(x)), y(static_cast<T>(y)) { this->zw() = zw; }

        template <typename U>
        _vec4(const _vec2<U>& xy, const _vec2<U>& zw) { this->xy() = xy; this->zw() = zw; }

        template <typename U, typename V>
        _vec4(const _vec3<U>& xyz, V w) : w(static_cast<T>(w)) { this->xyz() = xyz; }

        template <typename U, typename V>
        _vec4(U x, const _vec3<U>& yzw) : x(static_cast<T>(x)) { this->yzw() = yzw; }

        template <typename U>
        _vec4(const _vec4<U>& v) { this->xyzw() = v; }
        
        // Access
        DEFINE_ACCESSORS;
        
        // Infix and assignment operators
#define OP(op) inline _vec4 operator##op(const _vec4& v) const { return _vec4(x op v.x, y op v.y, z op v.z, w op v.w); } \
               inline _vec4 operator##op(T v) const { return _vec4(x op v, y op v, z op v, w op v); } \
               inline _vec4& operator##op##=(const _vec4& v) { x op##= v.x; y op##= v.y; z op##= v.z; w op##= v.w; return *this; } \
               inline _vec4& operator##op##=(T v) { x op##= v; y op##= v; z op##= v; w op##= v; return *this; }
        OP(+);
        OP(-);
        OP(*);
        OP(/);
#undef OP

        static _vec4<T> right;
        static _vec4<T> up;
        static _vec4<T> forward;

    private:
#include "vecref.inl"
    public:
#define VEC_HAS_Z
#define VEC_HAS_W
#include "swizzling.inl"
#undef VEC_HAS_W
#undef VEC_HAS_Z
    };

    template <typename T>
    _vec4<T> _vec4<T>::right = _vec4<T>(1, 0, 0, 0);
    template <typename T>
    _vec4<T> _vec4<T>::up = _vec4<T>(0, 1, 0, 0);
    template <typename T>
    _vec4<T> _vec4<T>::forward = _vec4<T>(0, 0, 1, 0);

#undef DEFINE_ACCESSORS

    template <typename T>
    std::ostream& operator<<(std::ostream& s, const _vec4<T>& v)
    {
        return s << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    }

    //
    // Type aliases
    //

    using bvec2 = _vec2<bool>;
    using bvec3 = _vec3<bool>;
    using bvec4 = _vec4<bool>;

    using ivec2 = _vec2<int>;
    using ivec3 = _vec3<int>;
    using ivec4 = _vec4<int>;

    using uvec2 = _vec2<unsigned int>;
    using uvec3 = _vec3<unsigned int>;
    using uvec4 = _vec4<unsigned int>;

    using vec2 = _vec2<float>;
    using vec3 = _vec3<float>;
    using vec4 = _vec4<float>;

    using dvec2 = _vec2<double>;
    using dvec3 = _vec3<double>;
    using dvec4 = _vec4<double>;
}
