#pragma once

#include <string>

#include "Empty/glad/glad.h"
#include "Empty/math/mat.h"
#include "Empty/math/vec.h"
#include "Empty/render/gl/ShaderProgram.hpp"

namespace render::gl
{
	struct UniformBase
	{
		std::string name;
		UniformBase(const std::string& n) : name(n) {}
		virtual void upload(location loc) = 0;
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
		void upload(location loc) override { exp; }                            \
	}

	GEN_UNIFORM_STRUCT(int, glUniform1i(loc, value));
	GEN_UNIFORM_STRUCT(math::ivec2, glUniform2i(loc, value.x, value.y));
	GEN_UNIFORM_STRUCT(math::ivec3, glUniform3i(loc, value.x, value.y, value.z));
	GEN_UNIFORM_STRUCT(math::ivec4, glUniform4i(loc, value.x, value.y, value.z, value.w));
	GEN_UNIFORM_STRUCT(float, glUniform1f(loc, value));
	GEN_UNIFORM_STRUCT(math::vec2, glUniform2f(loc, value.x, value.y));
	GEN_UNIFORM_STRUCT(math::vec3, glUniform3f(loc, value.x, value.y, value.z));
	GEN_UNIFORM_STRUCT(math::vec4, glUniform4f(loc, value.x, value.y, value.z, value.w));

	GEN_UNIFORM_STRUCT(math::mat2, glUniformMatrix2fv(loc, 1, false, value));
	GEN_UNIFORM_STRUCT(math::mat3, glUniformMatrix3fv(loc, 1, false, value));
	GEN_UNIFORM_STRUCT(math::mat4, glUniformMatrix4fv(loc, 1, false, value));

#undef GEN_UNIFORM_STRUCT
}
