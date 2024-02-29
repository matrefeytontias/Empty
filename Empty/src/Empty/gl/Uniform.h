#pragma once

#include <string>

#include "glad/glad.h"
#include "Empty/math/mat.h"
#include "Empty/math/vec.h"
#include "Empty/gl/ShaderProgram.hpp"

namespace Empty::gl
{
	struct UniformBase
	{
		std::string_view name;
		UniformBase(const std::string_view& n) : name(n) {}
		virtual void upload(const std::shared_ptr<ProgramId>&, location) = 0;
	};

	/**
	 * Helper class to facilitate uploading uniform data to a ShaderProgram.
	 */
	template <typename T>
	struct Uniform : public UniformBase {};

	// Generate a template specialization for the `Uniform` struct
	// with a provided upload function call.
#define GEN_UNIFORM_STRUCT(type, exp) template <> struct Uniform<type> : public UniformBase \
	{                                                                                       \
		Uniform(const std::string_view& n, const type& v) : UniformBase(n), value(v) {}     \
		type value;                                                                         \
		void upload(const std::shared_ptr<ProgramId>& id, location loc) override { exp; }                                                     \
	}

	GEN_UNIFORM_STRUCT(bool, glProgramUniform1i(*id, loc, value));
	GEN_UNIFORM_STRUCT(math::bvec2, glProgramUniform2i(*id, loc, value.x, value.y));
	GEN_UNIFORM_STRUCT(math::bvec3, glProgramUniform3i(*id, loc, value.x, value.y, value.z));
	GEN_UNIFORM_STRUCT(math::bvec4, glProgramUniform4i(*id, loc, value.x, value.y, value.z, value.w));
	GEN_UNIFORM_STRUCT(int, glProgramUniform1i(*id, loc, value));
	GEN_UNIFORM_STRUCT(math::ivec2, glProgramUniform2i(*id, loc, value.x, value.y));
	GEN_UNIFORM_STRUCT(math::ivec3, glProgramUniform3i(*id, loc, value.x, value.y, value.z));
	GEN_UNIFORM_STRUCT(math::ivec4, glProgramUniform4i(*id, loc, value.x, value.y, value.z, value.w));
	GEN_UNIFORM_STRUCT(unsigned int, glProgramUniform1ui(*id, loc, value));
	GEN_UNIFORM_STRUCT(math::uvec2, glProgramUniform2ui(*id, loc, value.x, value.y));
	GEN_UNIFORM_STRUCT(math::uvec3, glProgramUniform3ui(*id, loc, value.x, value.y, value.z));
	GEN_UNIFORM_STRUCT(math::uvec4, glProgramUniform4ui(*id, loc, value.x, value.y, value.z, value.w));
	GEN_UNIFORM_STRUCT(float, glProgramUniform1f(*id, loc, value));
	GEN_UNIFORM_STRUCT(math::vec2, glProgramUniform2f(*id, loc, value.x, value.y));
	GEN_UNIFORM_STRUCT(math::vec3, glProgramUniform3f(*id, loc, value.x, value.y, value.z));
	GEN_UNIFORM_STRUCT(math::vec4, glProgramUniform4f(*id, loc, value.x, value.y, value.z, value.w));

	GEN_UNIFORM_STRUCT(math::mat2, glProgramUniformMatrix2fv(*id, loc, 1, false, value));
	GEN_UNIFORM_STRUCT(math::mat3, glProgramUniformMatrix3fv(*id, loc, 1, false, value));
	GEN_UNIFORM_STRUCT(math::mat4, glProgramUniformMatrix4fv(*id, loc, 1, false, value));

#undef GEN_UNIFORM_STRUCT
}
