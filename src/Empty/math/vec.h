#pragma once

#include <memory>

namespace math
{
#define DEFINE_ACCESSORS T operator()(int k) const { return data[k]; } \
    T& operator()(int k) { return data[k]; } \
    T operator[](int k) const { return data[k]; } \
    T& operator[](int k) { return data[k]; } \
    operator const T* () const { return data; } \
    operator T* () { return data; }

    template <typename T>
    struct _vec2
    {
        using ElementType = T;
        union
        {
            struct { T x; T y; };
            T data[2];
        };
        // Constructors
        _vec2() = default;
        _vec2(T x, T y) : x(x), y(y) {}
        template <typename U>
        _vec2(U x, U y) : x(static_cast<T>(x)), y(static_cast<T>(y)) {}
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
        OP(/ );
#undef OP
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
            T data[3];
            struct { _vec2<T> xy; };
        };
        // Constructors
        _vec3() = default;
        _vec3(T x, T y, T z) : x(x), y(y), z(z) {}
        template <typename U>
        _vec3(U x, U y, U z) : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)) {}
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
        OP(/ );
#undef OP
    };

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
            T data[4];
            struct { _vec2<T> xy, zw; };
            struct { _vec3<T> xyz; };
        };
        // Constructors
        _vec4() = default;
        _vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
        template <typename U>
        _vec4(U x, U y, U z, U w) : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)), w(static_cast<T>(w)) {}
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
        OP(/ );
#undef OP
    };

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
