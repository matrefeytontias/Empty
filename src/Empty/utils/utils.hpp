#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Empty/math/Matrix.hpp"
#include "Empty/glad/glad.h"

#ifdef WIN32
#undef near
#undef far
#endif

namespace render::gl
{
	enum struct CubeMapFace : GLenum;
}

namespace utils
{
	/**
	 * Sets the working directory.
	 * @param   argv    the program's argv, retrieved through the main function
	 */
	void setwd(char** argv);
	/**
	 * Returns the contents of a file as a string. Raises a runtime_error exception
	 * upon failure.
	 */
	std::string getFileContents(const std::string& path);
	/**
	 * Splits a string on delimiting characters. The delimiting string is a disjunction,
	 * meaning the input string is split by either of the characters in the delimiter string.
	 */
	std::vector<std::string> split(const std::string& s, const std::string& delim);

	/**
	 * Prints all available OpenGL error codes.
	 */
	void checkGLerror(const char* file, int line);

#ifndef min
	/**
	 * Minimum of two values of the same comparable type.
	 */
	template <typename T> inline T min(T a, T b)
	{
		return a < b ? a : b;
	}
#endif

#ifndef max
	/**
	 * Maximum of two values of the same comparable type.
	 */
	template <typename T> inline T max(T a, T b)
	{
		return a > b ? a : b;
	}
#endif

	/**
	 * Constructs a 4x4 3D perspective matrix in the given matrix object based on FOV,
	 * aspect ratio, near plane and far plane.
	 */
	void perspective(math::Matrix4f& p, float fov, float ratio, float near, float far);
	/**
	 * Shorthand function to modify the aspect ratio of a perspective matrix.
	 */
	void setAspectRatio(math::Matrix4f& p, float ratio);

	/**
	 * Extract the underlying value of a scoped enum value
	 * From https://stackoverflow.com/a/14589519/5446815
	 */
	template <typename Enum>
	constexpr auto value(Enum e) -> typename std::underlying_type<Enum>::type
	{
		return static_cast<typename std::underlying_type<Enum>::type>(e);
	}

	/**
	 * Utility function to get the enum value corresponding to a cubemap face index.
	 */
	render::gl::CubeMapFace getCubeMapFaceFromIndex(int i);
}
