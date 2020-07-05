#pragma once

#include "Empty/math/mat.h"

/**
 * Various mathematical utility functions.
 */

namespace math
{
	/**
	 * Returns a matrix representing a rotation of a given angle around the X axis.
	 */
	mat4 rotateX(float a)
	{
		mat4 r = mat4::Identity();
		float c = cosf(a), s = sinf(a);
		r(1, 1) = r(2, 2) = c;
		r(1, 2) = -(r(2, 1) = s);
		return r;
	}

	/**
	 * Returns a matrix representing a rotation of a given angle around the Y axis.
	 */
	mat4 rotateY(float a)
	{
		mat4 r = mat4::Identity();
		float c = cosf(a), s = sinf(a);
		r(0, 0) = r(2, 2) = c;
		r(2, 0) = -(r(0, 2) = s);
		return r;
	}

	/**
	 * Returns a matrix representing a rotation of a given angle around the Z axis.
	 */
	mat4 rotateZ(float a)
	{
		mat4 r = mat4::Identity();
		float c = cosf(a), s = sinf(a);
		r(0, 0) = r(1, 1) = c;
		r(0, 1) = -(r(1, 0) = s);
		return r;
	}
}