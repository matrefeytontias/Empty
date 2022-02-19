#pragma once

#include "Empty/math/mat.h"
#include "Empty/math/vec.h"

/**
 * This file implements mathematical functions in a fashion consistent with GLSL.
 */

namespace math
{
	/// Scalar operators
	
	template <typename T> T min(T a, T b) { return a < b ? a : b; }
	template <typename T> T max(T a, T b) { return a < b ? b : a; }
	template <typename T> T clamp(T x, T a, T b) { return max(a, min(b, x)); }

	mat2 inverse(const mat2& m);
	dmat2 inverse(const dmat2& m);
	mat3 inverse(const mat3& m);
	dmat3 inverse(const dmat3& m);
	mat4 inverse(const mat4& m);
	dmat4 inverse(const dmat4& m);

#define VEC_FUNC(n) template <typename T> typename _vec##n<T>::concrete_vec
#define SCALAR_FUNC(n) template <typename T> typename _vec##n<T>::concrete_elem

	/// Vector operators

	SCALAR_FUNC(2) dot(const _vec2<T>& a, const _vec2<T>& b) { return a(0) * b(0) + a(1) * b(1); }
	SCALAR_FUNC(3) dot(const _vec3<T>& a, const _vec3<T>& b) { return a(0) * b(0) + a(1) * b(1) + a(2) * b(2); }
	SCALAR_FUNC(4) dot(const _vec4<T>& a, const _vec4<T>& b) { return a(0) * b(0) + a(1) * b(1) + a(2) * b(2) + a(3) * b(3); }

	VEC_FUNC(3) cross(const _vec3<T>& a, const _vec3<T>& b) { return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; }

	SCALAR_FUNC(2) length(const _vec2<T>& v) { return sqrt(dot(v, v)); }
	SCALAR_FUNC(3) length(const _vec3<T>& v) { return sqrt(dot(v, v)); }
	SCALAR_FUNC(4) length(const _vec4<T>& v) { return sqrt(dot(v, v)); }

	VEC_FUNC(2) normalize(const _vec2<T>& v) { return v / length(v); }
	VEC_FUNC(3) normalize(const _vec3<T>& v) { return v / length(v); }
	VEC_FUNC(4) normalize(const _vec4<T>& v) { return v / length(v); }

	VEC_FUNC(2) min(const _vec2<T>& a, const _vec2<T>& b) { return { min(a.x, b.x), min(a.y, b.y) }; }
	VEC_FUNC(3) min(const _vec3<T>& a, const _vec3<T>& b) { return { min(a.xy(), b.xy()), min(a.z, b.z) }; }
	VEC_FUNC(4) min(const _vec4<T>& a, const _vec4<T>& b) { return { min(a.xyz(), b.xyz()), min(a.w, b.w) }; }

	VEC_FUNC(2) max(const _vec2<T>& a, const _vec2<T>& b) { return { max(a.x, b.x), max(a.y, b.y) }; }
	VEC_FUNC(3) max(const _vec3<T>& a, const _vec3<T>& b) { return { max(a.xy(), b.xy()), max(a.z, b.z) }; }
	VEC_FUNC(4) max(const _vec4<T>& a, const _vec4<T>& b) { return { max(a.xyz(), b.xyz()), max(a.w, b.w) }; }
	
	VEC_FUNC(2) clamp(const _vec2<T>& v, const _vec2<T>& a, const _vec2<T>& b) { return { clamp(v.x, a.x, b.x), clamp(v.y, a.y, b.y) }; }
	VEC_FUNC(3) clamp(const _vec3<T>& v, const _vec3<T>& a, const _vec3<T>& b) { return { clamp(v.xy(), a.xy(), b.xy()), clamp(v.z, a.z, b.z) }; }
	VEC_FUNC(4) clamp(const _vec4<T>& v, const _vec4<T>& a, const _vec4<T>& b) { return { clamp(v.xyz(), a.xyz(), b.xyz()), clamp(v.w, a.w, b.w) }; }

	VEC_FUNC(2) clamp(const _vec2<T>& v, T a, T b) { return { clamp(v.x, a, b), clamp(v.y, a, b) }; }
	VEC_FUNC(3) clamp(const _vec3<T>& v, T a, T b) { return { clamp(v.xy(), a, b), clamp(v.z, a, b) }; }
	VEC_FUNC(4) clamp(const _vec4<T>& v, T a, T b) { return { clamp(v.xyz(), a, b), clamp(v.w, a, b) }; }

#undef SCALAR_FUNC
#undef VEC_FUNC
}
