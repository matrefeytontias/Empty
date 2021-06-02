#pragma once

#include "Empty/math/mat.h"
#include "Empty/math/vec.h"

/**
 * This file implements mathematical functions in a fashion consistent with GLSL.
 */

namespace math
{
	/// Vector operators

	template <typename T>
	T dot(const _vec2<T>& a, const _vec2<T>& b) { return a(0) * b(0) + a(1) * b(1); }
	template <typename T>
	T dot(const _vec3<T>& a, const _vec3<T>& b) { return a(0) * b(0) + a(1) * b(1) + a(2) * b(2); }
	template <typename T>
	T dot(const _vec4<T>& a, const _vec4<T>& b) { return a(0) * b(0) + a(1) * b(1) + a(2) * b(2) + a(3) * b(3); }

	template <typename T>
	_vec3<T> cross(const _vec3<T>& a, const _vec3<T>& b) { return _vec3<T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

#define VEC_FUNC(name, f, n) template <typename T> _vec##n<T> name(const _vec##n<T>& a) { f; }
#define VEC_FUNCS(name, f) VEC_FUNC(name, f, 2) \
VEC_FUNC(name, f, 3) \
VEC_FUNC(name, f, 4)

	VEC_FUNCS(normalize, return a / static_cast<T>(sqrt(dot(a, a))));
#undef VEC_FUNCS
#undef VEC_FUNC
}
