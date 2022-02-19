#pragma once

#include <iostream>

namespace math
{
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
        // Type of element without reference qualifier
        using concrete_elem = T;
        // Type of vector of concrete_elem
        using concrete_vec = _vec2;
        // Type of reference to element
        using ref_elem = std::reference_wrapper<T>;
        // Type of vector of ref_elem
        using ref_vec = _vec2<ref_elem>;

        union
        {
            struct { T x; T y; };
            struct { T r; T g; };
            struct { T s; T t; };
        };
        
        // Constructors
        _vec2() = default;

        _vec2(T x, T y) : x(x), y(y) {}
        
        template <typename U>
        _vec2(const _vec2<U>& xy) : x(xy.x), y(xy.y) {}

        template <typename U>
        _vec2& operator=(const _vec2<U>& v)
        {
            x = v.x;
            y = v.y;
            return *this;
        }
        
        // Access
        DEFINE_ACCESSORS;
        
        // Infix and assignment operators
#define OP(op) _vec2 operator##op(const _vec2& v) const { return _vec2(x op v.x, y op v.y); } \
               _vec2 operator##op(T v) const { return _vec2(x op v, y op v); } \
               _vec2& operator##op##=(const _vec2& v) { x op##= v.x; y op##= v.y; return *this; } \
               _vec2& operator##op##=(T v) { x op##= v; y op##= v; return *this; }
        OP(+);
        OP(-);
        OP(*);
        OP(/);
        _vec2 operator-() const { return _vec2(-x, -y); }
#undef OP

#include "swizzling.inl"
    };

    template <typename T>
    struct _vec3
    {
        // Type of element without reference qualifier
        using concrete_elem = T;
        // Type of vector of concrete_elem
        using concrete_vec = _vec3;
        // Type of reference to element
        using ref_elem = std::reference_wrapper<T>;
        // Type of vector of ref_elem
        using ref_vec = _vec3<ref_elem>;

        union
        {
            struct { T x; T y; T z; };
            struct { T r; T g; T b; };
            struct { T s; T t; T p; };
        };

        // Constructors
        _vec3() = default;
        _vec3(T x, T y, T z) : x(x), y(y), z(z) {}
        
        template <typename U>
        _vec3(const _vec2<U>& xy, T z) : x(xy.x), y(xy.y), z(z) {}
        
        template <typename U>
        _vec3(T x, const _vec2<U>& yz) : x(x), y(yz.x), z(yz.y) {}
        
        template <typename U>
        _vec3(const _vec3<U>& xyz) : x(xyz.x), y(xyz.y), z(xyz.z) {}

        template <typename U>
        _vec3& operator=(const _vec3<U>& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
            return *this;
        }
        
        // Access
        DEFINE_ACCESSORS;

        // Infix and assignment operators
#define OP(op) _vec3 operator##op(const _vec3& v) const { return _vec3(x op v.x, y op v.y, z op v.z); } \
               _vec3 operator##op(T v) const { return _vec3(x op v, y op v, z op v); } \
               _vec3& operator##op##=(const _vec3& v) { x op##= v.x; y op##= v.y; z op##= v.z; return *this; } \
               _vec3& operator##op##=(T v) { x op##= v; y op##= v; z op##= v; return *this; }
        OP(+);
        OP(-);
        OP(*);
        OP(/);
        _vec3 operator-() const { return _vec3(-x, -y, -z); }
#undef OP

        static _vec3<T> right;
        static _vec3<T> up;
        static _vec3<T> forward;

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
    struct _vec4
    {
        // Type of element without reference qualifier
        using concrete_elem = T;
        // Type of vector of concrete_elem
        using concrete_vec = _vec4;
        // Type of reference to element
        using ref_elem = std::reference_wrapper<T>;
        // Type of vector of ref_elem
        using ref_vec = _vec4<ref_elem>;
        
        union
        {
            struct { T x; T y; T z; T w; };
            struct { T r; T g; T b; T a; };
            struct { T s; T t; T p; T q; };
        };

        // Constructors
        _vec4() = default;

        _vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

        template <typename U>
        _vec4(const _vec2<U>& xy, T z, T w) : x(xy.x), y(xy.y), z(z), w(w) {}

        template <typename U>
        _vec4(T x, const _vec2<U>& yz, T w) : x(x), y(yz.x), z(yz.y), w(w) {}

        template <typename U>
        _vec4(T x, T y, const _vec2<U>& zw) : x(x), y(y), z(zw.x), w(zw.y) {}

        template <typename U>
        _vec4(const _vec2<U>& xy, const _vec2<U>& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}

        template <typename U>
        _vec4(const _vec3<U>& xyz, T w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

        template <typename U>
        _vec4(T x, const _vec3<U>& yzw) : x(x), y(yzw.x), z(yzw.y), w(yzw.z) {}

        template <typename U>
        _vec4(const _vec4<U>& xyzw) : x(xyzw.x), y(xyzw.y), z(xyzw.z), w(xyzw.w) {}

        template <typename U>
        _vec4& operator=(const _vec4<U>& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
            return *this;
        }
        
        // Access
        DEFINE_ACCESSORS;
        
        // Infix and assignment operators
#define OP(op) _vec4 operator##op(const _vec4& v) const { return _vec4(x op v.x, y op v.y, z op v.z, w op v.w); } \
               _vec4 operator##op(T v) const { return _vec4(x op v, y op v, z op v, w op v); } \
               _vec4& operator##op##=(const _vec4& v) { x op##= v.x; y op##= v.y; z op##= v.z; w op##= v.w; return *this; } \
               _vec4& operator##op##=(T v) { x op##= v; y op##= v; z op##= v; w op##= v; return *this; }
        OP(+);
        OP(-);
        OP(*);
        OP(/);
        _vec4 operator-() const { return _vec4(-x, -y, -z, -w); }
#undef OP

        static _vec4<T> right;
        static _vec4<T> up;
        static _vec4<T> forward;

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

    //
    // Stream operators
    //

    template <typename T>
    std::ostream& operator<<(std::ostream& s, const _vec2<T>& v)
    {
        return s << "(" << v.x << ", " << v.y << ")";
    }
    template <typename T>
    std::ostream& operator<<(std::ostream& s, const _vec3<T>& v)
    {
        return s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }
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

    //
    // Specializations for coordinate swizzling
    //

    /**
    * 2-component vector
    */
    template <typename T>
    struct _vec2<std::reference_wrapper<T>>
    {
        // Type of element without reference qualifier
        using concrete_elem = T;
        // Type of vector of concrete_elem
        using concrete_vec = _vec2<T>;
        // Type of reference to element
        using ref_elem = std::reference_wrapper<T>;
        // Type of vector of ref_elem
        using ref_vec = _vec2<ref_elem>;

        union
        {
            struct { ref_elem x; ref_elem y; };
            struct { ref_elem r; ref_elem g; };
            struct { ref_elem s; ref_elem t; };
        };

        // Constructors
        _vec2() = delete;
        _vec2(T& x, T& y) : x(x), y(y) {}
        _vec2(const _vec2&) = delete;

        operator concrete_vec() const { return { x.get(), y.get() }; }

        template <typename U>
        _vec2& operator=(const _vec2<U>& v)
        {
            // cast and prevent aliasing
            T tx = static_cast<T>(v.x), ty = static_cast<T>(v.y);
            x.get() = tx;
            y.get() = ty;
            return *this;
        }
        template <typename U>
        _vec2& operator=(_vec2<U>&& v)
        {
            // cast and prevent aliasing
            T tx = static_cast<T>(v.x), ty = static_cast<T>(v.y);
            x.get() = tx;
            y.get() = ty;
            return *this;
        }

        // Access
        DEFINE_ACCESSORS;

        // Infix and assignment operators
#define OP(op) concrete_vec operator##op(const concrete_vec& v) const { return concrete_vec(x op v.x, y op v.y); } \
               concrete_vec operator##op(concrete_elem v) const { return concrete_vec(x op v, y op v); } \
               _vec2& operator##op##=(const concrete_vec& v) { x op##= v.x; y op##= v.y; return *this; } \
               _vec2& operator##op##=(concrete_elem v) { x op##= v; y op##= v; return *this; }
        OP(+);
        OP(-);
        OP(*);
        OP(/);
        _vec2 operator-() const { return _vec2(-x, -y); }
#undef OP

#include "swizzling.inl"
    };

    template <typename T>
    struct _vec3<std::reference_wrapper<T>>
    {
        // Type of element without reference qualifier
        using concrete_elem = T;
        // Type of vector of concrete_elem
        using concrete_vec = _vec3<T>;
        // Type of reference to element
        using ref_elem = std::reference_wrapper<T>;
        // Type of vector of ref_elem
        using ref_vec = _vec3<ref_elem>;

        union
        {
            struct { ref_elem x; ref_elem y; ref_elem z; };
            struct { ref_elem r; ref_elem g; ref_elem b; };
            struct { ref_elem s; ref_elem t; ref_elem p; };
        };

        // Constructors
        _vec3() = delete;
        _vec3(T& x, T& y, T& z) : x(x), y(y), z(z) {}
        _vec3(const _vec3&) = delete;

        operator concrete_vec() const { return { x.get(), y.get(), z.get() }; }

        template <typename U>
        _vec3& operator=(const _vec3<U>& v)
        {
            // cast and prevent aliasing
            T tx = static_cast<T>(v.x), ty = static_cast<T>(v.y), tz = static_cast<T>(v.z);
            x.get() = tx;
            y.get() = ty;
            z.get() = tz;
            return *this;
        }
        template <typename U>
        _vec3& operator=(_vec3<U>&& v)
        {
            // cast and prevent aliasing
            T tx = static_cast<T>(v.x), ty = static_cast<T>(v.y), tz = static_cast<T>(v.z);
            x.get() = tx;
            y.get() = ty;
            z.get() = tz;
            return *this;
        }

        // Access
        DEFINE_ACCESSORS;

        // Infix and assignment operators
#define OP(op) concrete_vec operator##op(const concrete_vec& v) const { return concrete_vec(x op v.x, y op v.y, z op v.z); } \
               concrete_vec operator##op(concrete_elem v) const { return concrete_vec(x op v, y op v, z op v); } \
               _vec3& operator##op##=(const concrete_vec& v) { x op##= v.x; y op##= v.y; z op##= v.z; return *this; } \
               _vec3& operator##op##=(concrete_elem v) { x op##= v; y op##= v; z op##= v; return *this; }
        OP(+);
        OP(-);
        OP(*);
        OP(/);
        _vec3 operator-() const { return _vec3(-x, -y, -z); }
#undef OP

#define VEC_HAS_Z
#include "swizzling.inl"
#undef VEC_HAS_Z
    };

    template <typename T>
    struct _vec4<std::reference_wrapper<T>>
    {
        // Type of element without reference qualifier
        using concrete_elem = T;
        // Type of vector of concrete_elem
        using concrete_vec = _vec4<T>;
        // Type of reference to element
        using ref_elem = std::reference_wrapper<T>;
        // Type of vector of ref_elem
        using ref_vec = _vec4<ref_elem>;

        union
        {
            struct { ref_elem x; ref_elem y; ref_elem z; ref_elem w; };
            struct { ref_elem r; ref_elem g; ref_elem b; ref_elem a; };
            struct { ref_elem s; ref_elem t; ref_elem p; ref_elem q; };
        };

        // Constructors
        _vec4() = delete;
        _vec4(T& x, T& y, T& z, T& w) : x(x), y(y), z(z), w(w) {}
        _vec4(const _vec4&) = delete;

        operator concrete_vec() const { return { x.get(), y.get(), z.get(), w.get() }; }

        template <typename U>
        _vec4& operator=(const _vec4<U>& v)
        {
            // cast and prevent aliasing
            T tx = static_cast<T>(v.x), ty = static_cast<T>(v.y), tz = static_cast<T>(v.z), tw = static_cast<T>(v.w);
            x.get() = tx;
            y.get() = ty;
            z.get() = tz;
            w.get() = tw;
            return *this;
        }
        template <typename U>
        _vec4& operator=(_vec4<U>&& v)
        {
            // cast and prevent aliasing
            T tx = static_cast<T>(v.x), ty = static_cast<T>(v.y), tz = static_cast<T>(v.z), tw = static_cast<T>(v.w);
            x.get() = tx;
            y.get() = ty;
            z.get() = tz;
            w.get() = tw;
            return *this;
        }

        // Access
        DEFINE_ACCESSORS;

        // Infix and assignment operators
#define OP(op) concrete_vec operator##op(const concrete_vec& v) const { return concrete_vec(x op v.x, y op v.y, z op v.z, w op v.w); } \
               concrete_vec operator##op(concrete_elem v) const { return concrete_vec(x op v, y op v, z op v, w op v); } \
               _vec4& operator##op##=(const concrete_vec& v) { x op##= v.x; y op##= v.y; z op##= v.z; w op##= v.w; return *this; } \
               _vec4& operator##op##=(concrete_elem v) { x op##= v; y op##= v; z op##= v; w op##= v; return *this; }
        OP(+);
        OP(-);
        OP(*);
        OP(/);
        _vec4 operator-() const { return _vec4(-x, -y, -z, -w); }
#undef OP

#define VEC_HAS_Z
#define VEC_HAS_W
#include "swizzling.inl"
#undef VEC_HAS_W
#undef VEC_HAS_Z
    };

#undef DEFINE_ACCESSORS
    
}
