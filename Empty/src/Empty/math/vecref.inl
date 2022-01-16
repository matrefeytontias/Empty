template <typename T>
struct _vec2ref
{
    using MT = std::remove_const_t<T>;

    _vec2ref(T& x, T& y) : x(x), y(y) { }
    T& x;
    T& y;

    template <typename Type = T, std::enable_if_t<!std::is_const_v<Type>, int> = 0>
    _vec2ref& operator=(const _vec2<T>& a)
    {
        x = a.x;
        y = a.y;
        return *this;
    }

    operator _vec2<MT>() const { return _vec2<MT>(x, y); }

    // Infix and assignment operators
#define OP(op) inline _vec2<MT> operator##op(const _vec2<MT>& v) const { return _vec2(x op v.x, y op v.y); } \
               inline _vec2<MT> operator##op(MT v) const { return _vec2(x op v, y op v); } \
               inline _vec2ref& operator##op##=(const _vec2<MT>& v) { x op##= v.x; y op##= v.y; return *this; } \
               inline _vec2ref& operator##op##=(T v) { x op##= v; y op##= v; return *this; }
    OP(+);
    OP(-);
    OP(*);
    OP(/ );
#undef OP
};

template <typename T>
struct _vec3ref
{
    using MT = std::remove_const_t<T>;

    _vec3ref(T& x, T& y, T& z) : x(x), y(y), z(z) { }
    T& x;
    T& y;
    T& z;

    template <typename Type = T, std::enable_if_t<!std::is_const_v<Type>, int> = 0>
    _vec3ref& operator=(const _vec3<T>& a)
    {
        x = a.x;
        y = a.y;
        z = a.z;
        return *this;
    }

    operator _vec3<MT>() const { return _vec3<MT>(x, y, z); }

    // Infix and assignment operators
#define OP(op) inline _vec3<MT> operator##op(const _vec3<MT>& v) const { return _vec3(x op v.x, y op v.y, z op v.z); } \
               inline _vec3<MT> operator##op(T v) const { return _vec3(x op v, y op v, z op v); } \
               inline _vec3ref& operator##op##=(const _vec3<MT>& v) { x op##= v.x; y op##= v.y; z op##= v.z; return *this; } \
               inline _vec3ref& operator##op##=(T v) { x op##= v; y op##= v; z op##= v; return *this; }
    OP(+);
    OP(-);
    OP(*);
    OP(/ );
#undef OP
};

template <typename T>
struct _vec4ref
{
    using MT = std::remove_const_t<T>;

    _vec4ref(T& x, T& y, T& z, T& w) : x(x), y(y), z(z), w(w) { }
    T& x;
    T& y;
    T& z;
    T& w;

    template <typename Type = T, std::enable_if_t<!std::is_const_v<Type>, int> = 0>
    _vec4ref& operator=(const _vec4<T>& a)
    {
        x = a.x;
        y = a.y;
        z = a.z;
        w = a.w;
        return *this;
    }

    operator _vec4<MT>() const { return _vec4<MT>(x, y, z, w); }

    // Infix and assignment operators
#define OP(op) inline _vec4<MT> operator##op(const _vec4<MT>& v) const { return _vec4(x op v.x, y op v.y, z op v.z, w op v.w); } \
               inline _vec4<MT> operator##op(T v) const { return _vec4(x op v, y op v, z op v, w op v); } \
               inline _vec4ref& operator##op##=(const _vec4<MT>& v) { x op##= v.x; y op##= v.y; z op##= v.z; w op##= v.w; return *this; } \
               inline _vec4ref& operator##op##=(T v) { x op##= v; y op##= v; z op##= v; w op##= v; return *this; }
    OP(+);
    OP(-);
    OP(*);
    OP(/ );
#undef OP
};
