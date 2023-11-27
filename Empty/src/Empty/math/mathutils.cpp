#include "Empty/math/funcs.h"
#include "Empty/math/mathutils.hpp"

auto Empty::math::scale(float s) -> mat4
{
	mat4 m = mat4::Constant(0);
	m(0, 0) = s;
	m(1, 1) = s;
	m(2, 2) = s;
	m(3, 3) = 1.f;
	return m;
}

auto Empty::math::scale(vec3 s) -> mat4
{
	mat4 m = mat4::Constant(0);
	m(0, 0) = s.x;
	m(1, 1) = s.y;
	m(2, 2) = s.z;
	m(3, 3) = 1.f;
	return m;
}

auto Empty::math::rotateX(float a) -> mat4
{
	mat4 r = mat4::Identity();
	float c = cosf(a), s = sinf(a);
	r(1, 1) = r(2, 2) = c;
	r(1, 2) = -(r(2, 1) = s);
	return r;
}

auto Empty::math::rotateY(float a) -> mat4
{
	mat4 r = mat4::Identity();
	float c = cosf(a), s = sinf(a);
	r(0, 0) = r(2, 2) = c;
	r(2, 0) = -(r(0, 2) = s);
	return r;
}

auto Empty::math::rotateZ(float a) -> mat4
{
	mat4 r = mat4::Identity();
	float c = cosf(a), s = sinf(a);
	r(0, 0) = r(1, 1) = c;
	r(0, 1) = -(r(1, 0) = s);
	return r;
}

auto Empty::math::lookAt(vec3 forward, vec3 up) -> mat4
{
	vec3 x = normalize(cross(up, forward)),
		y = normalize(cross(forward, x));
	mat4 r;
	r.column(0) = vec4(x, 0);
	r.column(1) = vec4(y, 0);
	r.column(2) = vec4(forward, 0);
	r.column(3) = vec4(0, 0, 0, 1);
	return r;
}

auto Empty::math::lookAt(vec3 origin, vec3 target, vec3 up) -> mat4
{
	vec3 z = normalize(target - origin),
		x = normalize(cross(up, z)),
		y = normalize(cross(z, x));
	mat4 r;
	r.column(0) = vec4(x, 0);
	r.column(1) = vec4(y, 0);
	r.column(2) = vec4(z, 0);
	r.column(3) = vec4(origin, 1);
	return r;
}
