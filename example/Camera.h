#pragma once

#include "Empty/math/mat.h"
#include "Empty/math/mathutils.h"
#include "Empty/utils/utils.hpp"

struct Camera
{
	float movementSpeed = .05f, angularSpeed = 0.002f;
	math::mat4 m;
	math::mat4 p;

	Camera(float fov, float ratio, float near, float far) : m(math::mat4::Identity()), p(math::mat4::Identity()) { utils::perspective(p, fov, ratio, near, far); }

	void setPosition(float x, float y, float z)
	{
		m(0, 3) = x; m(1, 3) = y; m(2, 3) = z;
	}

	void translate(const math::vec3& v)
	{
		m(0, 3) += v.x; m(1, 3) += v.y; m(2, 3) += v.z;
	}

	void processInput(bool forward, bool back, bool up, bool down, bool left, bool right, float mouseDX, float mouseDY)
	{
		math::vec3 dir;
		dir.x = utils::select(movementSpeed, right) - utils::select(movementSpeed, left);
		dir.y = utils::select(movementSpeed, up) - utils::select(movementSpeed, down);
		dir.z = utils::select(movementSpeed, back) - utils::select(movementSpeed, forward);

		translate((m * math::vec4(dir.x, dir.y, dir.z, 0.f)).xyz);

		if (mouseDX)
			m *= math::rotateY(-mouseDX * angularSpeed);
		if (mouseDY)
			m *= math::rotateX(-mouseDY * angularSpeed);
	}
};
