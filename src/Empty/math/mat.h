#pragma once

#include "Empty/math/funcs.h"
#include "Empty/math/vec.h"

namespace math
{
#define DEFINE_FIELDS(n) _mat##n() = default;                      \
    static _mat##n Constant(T v)                                   \
    {                                                              \
        _mat##n r;                                                 \
        for (int k = 0; k < n * n; k++)                            \
            r._data[k] = v;                                        \
        return r;                                                  \
    }                                                              \
    static _mat##n Identity()                                      \
    {                                                              \
        _mat##n r;                                                 \
        for (int k = 0; k < n * n; k++)                            \
            r._data[k] = static_cast<T>(k % (n + 1) == 0 ? 1 : 0); \
        return r;                                                  \
    }                                                              \
    T operator[](int k) const { return _data[k]; }                 \
    T& operator[](int k) { return _data[k]; }                      \
    T operator()(int i, int j) const { return _data[i + j * n]; }  \
    T& operator()(int i, int j) { return _data[i + j * n]; }       \
    operator const T* () const { return _data; }                   \
    operator T* () { return _data; }

#define SCALAR_OP(n, op, type, var, subscript) _mat##n operator##op(const type var) const \
    { \
        _mat##n r; \
        for (int k = 0; k < n * n; k++) \
            r[k] = _data[k] op var##subscript; \
        return r; \
    }
#define SCALAR_OP_ASSIGN(n, op, type, var, subscript) _mat##n& operator##op(const type var)\
    { \
        for (int k = 0; k < n * n; k++) \
            _data[k] op##= var##subscript; \
        return *this; \
    }
#define MATRIX_MULT(n) _mat##n operator*(const _mat##n& a) const \
    { \
        _mat##n r; \
        for (int i = 0; i < n; i++) \
            for (int j = 0; j < n; j++) \
                r(i, j) = dot(row(i), a.column(j)); \
        return r; \
    }
#define VECTOR_MULT(n) _vec##n<T> operator*(const _vec##n<T>& a) const \
    { \
        _vec##n<T> r; \
        for (int i = 0; i < n; i++) \
            r(i) = dot(row(i), a); \
        return r; \
    }
    /**
     * Class for 2x2 matrices. Storage is column-major, in compliance with OpenGL requirements.
     */
    template <typename T>
    struct _mat2
    {
        // Constructors and accessors
        DEFINE_FIELDS(2);

        _vec2<T> column(int i) const { return _cols[i]; }
        _vec2<T>& column(int i) { return _cols[i]; }
        _vec2<T> row(int i) const { return _vec2<T>(_data[i], _data[i + 2]); }

        // Infix operators
        SCALAR_OP(2, +, T, v,);
        SCALAR_OP(2, +, _mat2&, a, [k]);
        SCALAR_OP(2, -, T, v, );
        SCALAR_OP(2, -, _mat2&, a, [k]);
        SCALAR_OP(2, *, T, v, );
        SCALAR_OP(2, /, T, v, );

        MATRIX_MULT(2);
        VECTOR_MULT(2);

        // Assignment operators
        SCALAR_OP_ASSIGN(2, +, T, v, );
        SCALAR_OP_ASSIGN(2, +, _mat2&, a, [k]);
        SCALAR_OP_ASSIGN(2, -, T, v, );
        SCALAR_OP_ASSIGN(2, -, _mat2&, a, [k]);
        SCALAR_OP_ASSIGN(2, *, T, v, );
        SCALAR_OP_ASSIGN(2, / , T, v, );
        
        _mat2& operator*=(const _mat2& a)
        {
            return *this = *this * a;
        }
    protected:
        union
        {
            T _data[4];
            struct { _vec2<T> _cols[2]; };
        };
    };

    /**
     * Class for 3x3 matrices. Storage is column-major, in compliance with OpenGL requirements.
     */
    template <typename T>
    struct _mat3
    {
        // Constructors and accessors
        DEFINE_FIELDS(3);

        _vec3<T> column(int i) const { return _cols[i]; }
        _vec3<T>& column(int i) { return _cols[i]; }
        _vec3<T> row(int i) const { return _vec3<T>(_data[i], _data[i + 3], _data[i + 6]); }

        // Infix operators
        SCALAR_OP(3, +, T, v, );
        SCALAR_OP(3, +, _mat3&, a, [k]);
        SCALAR_OP(3, -, T, v, );
        SCALAR_OP(3, -, _mat3&, a, [k]);
        SCALAR_OP(3, *, T, v, );
        SCALAR_OP(3, / , T, v, );

        MATRIX_MULT(3);
        VECTOR_MULT(3);

        // Assignment operators
        SCALAR_OP_ASSIGN(3, +, T, v, );
        SCALAR_OP_ASSIGN(3, +, _mat3&, a, [k]);
        SCALAR_OP_ASSIGN(3, -, T, v, );
        SCALAR_OP_ASSIGN(3, -, _mat3&, a, [k]);
        SCALAR_OP_ASSIGN(3, *, T, v, );
        SCALAR_OP_ASSIGN(3, / , T, v, );

        _mat3& operator*=(const _mat3& a)
        {
            return *this = *this * a;
        }
    protected:
        union
        {
            T _data[9];
            struct { _vec3<T> _cols[3]; };
        };
    };

    /**
     * Class for 4x4 matrices. Storage is column-major, in compliance with OpenGL requirements.
     */
    template <typename T>
    struct _mat4
    {
        // Constructors and accessors
        DEFINE_FIELDS(4);

        _vec4<T> column(int i) const { return _cols[i]; }
        _vec4<T>& column(int i) { return _cols[i]; }
        _vec4<T> row(int i) const { return _vec4<T>(_data[i], _data[i + 4], _data[i + 8], _data[i + 12]); }

        // Infix operators
        SCALAR_OP(4, +, T, v, );
        SCALAR_OP(4, +, _mat4&, a, [k]);
        SCALAR_OP(4, -, T, v, );
        SCALAR_OP(4, -, _mat4&, a, [k]);
        SCALAR_OP(4, *, T, v, );
        SCALAR_OP(4, / , T, v, );

        MATRIX_MULT(4);
        VECTOR_MULT(4);

        // Assignment operators
        SCALAR_OP_ASSIGN(4, +, T, v, );
        SCALAR_OP_ASSIGN(4, +, _mat4&, a, [k]);
        SCALAR_OP_ASSIGN(4, -, T, v, );
        SCALAR_OP_ASSIGN(4, -, _mat4&, a, [k]);
        SCALAR_OP_ASSIGN(4, *, T, v, );
        SCALAR_OP_ASSIGN(4, / , T, v, );

        _mat4& operator*=(const _mat4& a)
        {
            return *this = *this * a;
        }
    protected:
        union
        {
            T _data[16];
            struct { _vec4<T> _cols[4]; };
        };
    };

#define PRINT_OP(n) template <typename T> std::ostream& operator<<(std::ostream& s, const _mat##n<T>& m) \
    { \
        for (int i = 0; i < n; i++) \
        { \
            s << "("; \
            for (int j = 0; j < n; j++) \
                s << m(i, j) << (j == n - 1 ? ")" : ", "); \
            s << std::endl; \
        } \
        return s; \
    }

    PRINT_OP(2);
    PRINT_OP(3);
    PRINT_OP(4);

    using mat2 = _mat2<float>;
    using mat3 = _mat3<float>;
    using mat4 = _mat4<float>;

    using dmat2 = _mat2<double>;
    using dmat3 = _mat3<double>;
    using dmat4 = _mat4<double>;
}
