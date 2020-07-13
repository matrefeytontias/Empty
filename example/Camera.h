#pragma once

#include "Empty/math/mat.h"
#include "Empty/math/mathutils.hpp"
#include "Empty/utils/utils.hpp"

struct Camera
{
	float movementSpeed = .05f, angularSpeed = 0.002f;
	math::mat4 m;
	math::mat4 p;

	Camera(float fov, float ratio, float near, float far) : m(math::mat4::Identity()), p(math::mat4::Identity()) { utils::perspective(p, fov, ratio, near, far); }

	math::vec3 getPosition() const { return m.column(3).xyz; }
	void setPosition(float x, float y, float z) { m(0, 3) = x; m(1, 3) = y; m(2, 3) = z; }

	void translate(const math::vec3& v) { m.column(3).xyz += (m * math::vec4(v, 0)).xyz; }

	void processInput(bool forward, bool back, bool up, bool down, bool left, bool right, float mouseDX, float mouseDY)
	{
		math::vec3 dir;
		dir.x = utils::select(movementSpeed, right) - utils::select(movementSpeed, left);
		dir.y = utils::select(movementSpeed, up) - utils::select(movementSpeed, down);
		dir.z = utils::select(movementSpeed, back) - utils::select(movementSpeed, forward);

		translate(dir);

		if (mouseDX || mouseDY)
		{
			_xz -= mouseDX * angularSpeed;
			_yz += mouseDY * angularSpeed;
			math::vec3 look(sinf(_xz) * cosf(_yz), sinf(_yz), cosf(_xz) * cosf(_yz));
			auto p = getPosition();
			m = math::lookAt(look, math::vec3::up);
			setPosition(p.x, p.y, p.z);
		}
	}

private:
	float _xz = 0.f, _yz = 0.f;
};
