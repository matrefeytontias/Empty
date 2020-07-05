#pragma once

#include "Empty/math/mat.h"
#include "Empty/math/vec.h"

/**
 * This file implements mathematical functions in a fashion consistent with GLSL.
 */

namespace math
{
	template <typename T>
	T dot(const _vec2<T>& a, const _vec2<T>& b) { return a(0) * b(0) + a(1) * b(1); }
	template <typename T>
	T dot(const _vec3<T>& a, const _vec3<T>& b) { return a(0) * b(0) + a(1) * b(1) + a(2) * b(2); }
	template <typename T>
	T dot(const _vec4<T>& a, const _vec4<T>& b) { return a(0) * b(0) + a(1) * b(1) + a(2) * b(2) + a(3) * b(3); }
}