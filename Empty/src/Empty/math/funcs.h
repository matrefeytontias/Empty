#pragma once

#include "Empty/math/mat.h"
#include "Empty/math/vec.h"
#include "Empty/utils/typeutils.h"

/**
 * This file implements mathematical functions in a fashion consistent with GLSL.
 */

namespace Empty::math
{
	/// Scalar operators

#define SIGNATURE template <typename T, typename U> std::enable_if_t<std::is_same_v<utils::unwrapped_ref_t<T>, utils::unwrapped_ref_t<U>>, utils::unwrapped_ref_t<T>>
	
	SIGNATURE min(T a, U b) { return a < b ? a : b; }
	SIGNATURE max(T a, U b) { return a < b ? b : a; }
	SIGNATURE clamp(T x, U a, U b) { return max(a, min(b, x)); }

#undef SIGNATURE

	/// Vector operators

	// 1-argument functions

#define VEC_FUNC(n) template <typename T> typename _vec##n<T>::concrete_vec
#define SCALAR_FUNC(n) template <typename T> typename _vec##n<T>::concrete_elem

	SCALAR_FUNC(2) length(const _vec2<T>& v) { return sqrt(dot(v, v)); }
	SCALAR_FUNC(3) length(const _vec3<T>& v) { return sqrt(dot(v, v)); }
	SCALAR_FUNC(4) length(const _vec4<T>& v) { return sqrt(dot(v, v)); }

	VEC_FUNC(2) normalize(const _vec2<T>& v) { return v / length(v); }
	VEC_FUNC(3) normalize(const _vec3<T>& v) { return v / length(v); }
	VEC_FUNC(4) normalize(const _vec4<T>& v) { return v / length(v); }

#undef SCALAR_FUNC
#undef VEC_FUNC

	// 2-argument functions

#define VEC_FUNC(n) template <typename T, typename U> std::enable_if_t<std::is_same_v< \
	typename _vec##n<T>::concrete_elem, typename _vec##n<U>::concrete_elem>, typename _vec##n<T>::concrete_vec>
#define SCALAR_FUNC(n) template <typename T, typename U> std::enable_if_t<std::is_same_v< \
	typename _vec##n<T>::concrete_elem, typename _vec##n<U>::concrete_elem>, typename _vec##n<T>::concrete_elem>

	SCALAR_FUNC(2) dot(const _vec2<T>& a, const _vec2<U>& b) { return a(0) * b(0) + a(1) * b(1); }
	SCALAR_FUNC(3) dot(const _vec3<T>& a, const _vec3<U>& b) { return a(0) * b(0) + a(1) * b(1) + a(2) * b(2); }
	SCALAR_FUNC(4) dot(const _vec4<T>& a, const _vec4<U>& b) { return a(0) * b(0) + a(1) * b(1) + a(2) * b(2) + a(3) * b(3); }

	VEC_FUNC(3) cross(const _vec3<T>& a, const _vec3<U>& b) { return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; }

	VEC_FUNC(2) min(const _vec2<T>& a, const _vec2<U>& b) { return { min(a.x, b.x), min(a.y, b.y) }; }
	VEC_FUNC(3) min(const _vec3<T>& a, const _vec3<U>& b) { return { min(a.xy(), b.xy()), min(a.z, b.z) }; }
	VEC_FUNC(4) min(const _vec4<T>& a, const _vec4<U>& b) { return { min(a.xyz(), b.xyz()), min(a.w, b.w) }; }

	VEC_FUNC(2) max(const _vec2<T>& a, const _vec2<U>& b) { return { max(a.x, b.x), max(a.y, b.y) }; }
	VEC_FUNC(3) max(const _vec3<T>& a, const _vec3<U>& b) { return { max(a.xy(), b.xy()), max(a.z, b.z) }; }
	VEC_FUNC(4) max(const _vec4<T>& a, const _vec4<U>& b) { return { max(a.xyz(), b.xyz()), max(a.w, b.w) }; }
	
	// 3-argument functions

	VEC_FUNC(2) clamp(const _vec2<T>& v, const _vec2<U>& a, const _vec2<U>& b) { return { clamp(v.x, a.x, b.x), clamp(v.y, a.y, b.y) }; }
	VEC_FUNC(3) clamp(const _vec3<T>& v, const _vec3<U>& a, const _vec3<U>& b) { return { clamp(v.xy(), a.xy(), b.xy()), clamp(v.z, a.z, b.z) }; }
	VEC_FUNC(4) clamp(const _vec4<T>& v, const _vec4<U>& a, const _vec4<U>& b) { return { clamp(v.xyz(), a.xyz(), b.xyz()), clamp(v.w, a.w, b.w) }; }

	VEC_FUNC(2) clamp(const _vec2<T>& v, U a, U b) { return { clamp(v.x, a, b), clamp(v.y, a, b) }; }
	VEC_FUNC(3) clamp(const _vec3<T>& v, U a, U b) { return { clamp(v.xy(), a, b), clamp(v.z, a, b) }; }
	VEC_FUNC(4) clamp(const _vec4<T>& v, U a, U b) { return { clamp(v.xyz(), a, b), clamp(v.w, a, b) }; }

	/// Matrix operators

	mat2 inverse(const mat2& m);
	dmat2 inverse(const dmat2& m);
	mat3 inverse(const mat3& m);
	dmat3 inverse(const dmat3& m);
	mat4 inverse(const mat4& m);
	dmat4 inverse(const dmat4& m);

	template <typename T>
	_mat2<T> transpose(const _mat2<T>& m)
	{
		_mat2<T> r;
		r.column(0) = m.row(0);
		r.column(1) = m.row(1);
		return r;
	}

	template <typename T>
	_mat3<T> transpose(const _mat3<T>& m)
	{
		_mat3<T> r;
		r.column(0) = m.row(0);
		r.column(1) = m.row(1);
		r.column(2) = m.row(2);
		return r;
	}

	template <typename T>
	_mat4<T> transpose(const _mat4<T>& m)
	{
		_mat4<T> r;
		r.column(0) = m.row(0);
		r.column(1) = m.row(1);
		r.column(2) = m.row(2);
		r.column(3) = m.row(3);
		return r;
	}

#undef SCALAR_FUNC
#undef VEC_FUNC
}
