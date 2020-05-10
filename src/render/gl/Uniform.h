#pragma once

#include <string>

#include <glad/glad.h>

#include <math/Vector.hpp>
#include <math/Matrix.hpp>

namespace render::gl
{
	struct UniformBase
	{
		std::string name;
		UniformBase(const std::string& n) : name(n) {}
		virtual void upload(int location) = 0;
	};

	/**
	 * Helper class to facilitate uploading uniform data to a shader program.
	 */
	template <typename T>
	struct Uniform : public UniformBase {};

	// Generate a template specialization for the `Uniform` struct
	// with a provided upload function call.
#define GEN_UNIFORM_STRUCT(type, exp) template <> struct Uniform<type> : public UniformBase \
	{                                                                                       \
		Uniform(const std::string& n, const type& v) : UniformBase(n), value(v) {}          \
		type value;                                                                         \
		void upload(int location) override { exp; }                                         \
	}

	GEN_UNIFORM_STRUCT(int, glUniform1i(location, value));
	GEN_UNIFORM_STRUCT(math::Vector2i, glUniform2i(location, value.x(), value.y()));
	GEN_UNIFORM_STRUCT(math::Vector3i, glUniform3i(location, value.x(), value.y(), value.z()));
	GEN_UNIFORM_STRUCT(math::Vector4i, glUniform4i(location, value.x(), value.y(), value.z(), value.w()));
	GEN_UNIFORM_STRUCT(float, glUniform1f(location, value));
	GEN_UNIFORM_STRUCT(math::Vector2f, glUniform2f(location, value.x(), value.y()));
	GEN_UNIFORM_STRUCT(math::Vector3f, glUniform3f(location, value.x(), value.y(), value.z()));
	GEN_UNIFORM_STRUCT(math::Vector4f, glUniform4f(location, value.x(), value.y(), value.z(), value.w()));

	GEN_UNIFORM_STRUCT(math::Matrix3f, glUniformMatrix3fv(location, 1, false, value));
	GEN_UNIFORM_STRUCT(math::Matrix4f, glUniformMatrix4fv(location, 1, false, value));
}
