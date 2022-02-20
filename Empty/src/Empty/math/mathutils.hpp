#pragma once

#include "Empty/math/mat.h"

/**
 * Various mathematical utility functions.
 */

namespace Empty::math
{
	/**
	 * Returns a matrix representing a rotation of a given angle around the X axis.
	 */
	mat4 rotateX(float a);

	/**
	 * Returns a matrix representing a rotation of a given angle around the Y axis.
	 */
	mat4 rotateY(float a);

	/**
	 * Returns a matrix representing a rotation of a given angle around the Z axis.
	 */
	mat4 rotateZ(float a);

	/**
	 * Returns a matrix that transforms the Z axis into a given vector. This
	 * makes it so that transforming a vector with this matrix makes it "look"
	 * in a direction.
	 */
	mat4 lookAt(vec3 forward, vec3 up);

	/**
	 * Returns a matrix that makes the Z axis look at a given target. This
	 * makes it so that transforming a vector with this matrix makes it "look at"
	 * a target, while standing at `origin`.
	 */
	mat4 lookAt(vec3 origin, vec3 target, vec3 up);
}
